#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#include "driver/ledc.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "soc/cpu.h"

#define FADE_DURATION_MS (3000)
#define FADE_UP_TARGET (1.0f)
#define FADE_DOWN_TARGET (0.0f)

#define TIMER_DIVIDER (32768)
#define TIMER_SCALE ((1.0f * TIMER_BASE_CLK) / TIMER_DIVIDER)
#define TIMER_IDX_0 (0)
#define INITIAL_TIMER_TICKS ((uint32_t)((FADE_DURATION_MS / 1000.0) * TIMER_SCALE))

#define timer_ticks_from_ms(ms) ((uint32_t)((ms / 1000.0) * TIMER_SCALE))

#define TRIAC_IO    (2)
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<TRIAC_IO))
#define ZERO_DETECT_IO     (13)
#define GPIO_INPUT_PIN_SEL  ((1ULL<<ZERO_DETECT_IO))
#define ESP_INTR_FLAG_DEFAULT (0)
#define TRIAC_OFF (1)
#define TRIAC_ON (0)

#define AC_FREQ_HZ (120)
// full wave = 8.3repeating milliseconds
#define FULL_WAVE_PERIOD_MS (1000.0 / AC_FREQ_HZ)
#define THREE_QUARTER_WAVE_PERIOD_MS (3.0 * FULL_WAVE_PERIOD_MS / 4.0)
#define HALF_WAVE_PERIOD_MS (FULL_WAVE_PERIOD_MS / 2.0)
#define QUARTER_WAVE_PERIOD_MS (FULL_WAVE_PERIOD_MS / 4.0)

#define PWM_DUTY_RESOLUTION (LEDC_TIMER_19_BIT)

#define DIMMER_INTENSITY (FADE_UP_TARGET)

int triac_off_time = 1;
int zero_cross_count = 0;
int cnt = 0;
int level = 0;
bool zero_cross = false;

const ledc_timer_config_t ledc_timer = {
    .duty_resolution = PWM_DUTY_RESOLUTION, // resolution of PWM duty
    .freq_hz = 120,                      // frequency of PWM signal
    .speed_mode = LEDC_LOW_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_1            // timer index
};

const ledc_channel_config_t ledc_channel = {
    .channel = LEDC_CHANNEL_2,
    .duty = (uint32_t)((1 << PWM_DUTY_RESOLUTION) * DIMMER_INTENSITY),
    .gpio_num = TRIAC_IO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_sel = LEDC_TIMER_1
};

static xQueueHandle gpio_evt_queue = NULL;

int timer_isr_counter = 0, timer_isr_cleared = 0;
static void IRAM_ATTR timer_isr(void *arg) {
  int timer_idx = (int) arg;

  /* Retrieve the interrupt status and the counter value
  *        from the timer that reported the interrupt */
  uint32_t intr_status = TIMERG0.int_st_timers.val;
  TIMERG0.hw_timer[timer_idx].update = 1;
  uint64_t timer_counter_value =
    ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
    | TIMERG0.hw_timer[timer_idx].cnt_low;

  timer_isr_counter += 1;

  /* Clear the interrupt
  *        and update the alarm time for the
  *        timer with without reload */
  /*
  if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
    // evt.type = TEST_WITHOUT_RELOAD;
    TIMERG0.int_clr_timers.t0 = 1;
    timer_counter_value += (uint64_t) (TIMER_INTERVAL0_SEC * TIMER_SCALE);
    TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
    TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
  } else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
    // evt.type = TEST_WITH_RELOAD;
    TIMERG0.int_clr_timers.t1 = 1;
  } else {
    // evt.type = -1; // not supported even type
  }
  */
  if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_IDX_0) {
    TIMERG0.int_clr_timers.t0 = 1;
    timer_counter_value += (uint64_t) timer_ticks_from_ms(HALF_WAVE_PERIOD_MS);
    TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
    TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
    timer_isr_cleared += 1;
  }

  /* After the alarm has been triggered
  *       we need enable it again, so it
  *       is triggered the next time */
  TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

  /* Now just send the event data
  * back to the main program task */
  // xQueueSendFromISR(timer_queue, &evt, NULL);
}

void kick_off_timer(uint32_t period_ms) {
  timer_set_counter_value(TIMER_GROUP_0, TIMER_IDX_0, 0);
  printf("setting alarm value %u\n", timer_ticks_from_ms(period_ms));
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_IDX_0, timer_ticks_from_ms(period_ms));
  timer_enable_intr(TIMER_GROUP_0, TIMER_IDX_0);
  timer_isr_register(TIMER_GROUP_0, TIMER_IDX_0, timer_isr, (void *)TIMER_IDX_0, ESP_INTR_FLAG_IRAM, NULL);
  timer_start(TIMER_GROUP_0, TIMER_IDX_0);
}

int total_zero_cross = 0;
int current_wave = FULL_WAVE_PERIOD_MS;
static void IRAM_ATTR gpio_isr_handler(void *arg) {
  for (int i = 0; i < 100; i += 1); // debounce
  if (gpio_get_level(ZERO_DETECT_IO)) {

    // at a zero crossing, start the timer
    // kick_off_timer(HALF_WAVE_PERIOD_MS);

    zero_cross_count ++;
    total_zero_cross ++;
  }
}

int fade_direction = 1;
int fade_loops = 0;
static void IRAM_ATTR gpio_isr_handler_OLD(void *arg) {
    // Delay very slightly and retest pin to avoid bounce on negative edge.
    for (int i = 0; i < 100; ++i) {}
    if (gpio_get_level(ZERO_DETECT_IO)) {
        // ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);

        // ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        // Zero the PWM timer at the zero crossing.
        ledc_timer_rst(ledc_channel.speed_mode, ledc_channel.timer_sel);

        // 2* is just to hold at the period
        if (((total_zero_cross /*+ 1*/) % ((2 * FADE_DURATION_MS * AC_FREQ_HZ) / 1000)) == 0) {
          fade_loops += 1;

          fade_direction = 1 - fade_direction;

          ledc_set_fade_with_time(
            ledc_channel.speed_mode
            , ledc_channel.channel
            , (uint32_t)((1 << PWM_DUTY_RESOLUTION) * ((fade_direction == 1) ? FADE_UP_TARGET : FADE_DOWN_TARGET))
            , FADE_DURATION_MS
          );
          ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        }

        zero_cross_count ++;
        total_zero_cross ++;
    }
}

static void gpio_task_example(void* arg)
{
    gpio_set_level(TRIAC_IO, TRIAC_OFF);

    kick_off_timer(FULL_WAVE_PERIOD_MS);

    for(;;) {

      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    // configure triac output
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // configure zero-cross detector input
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // setup triac out timer
    timer_config_t timer_config;
    timer_config.divider = TIMER_DIVIDER; // max division, gets us down to 1.22KHz
    timer_config.counter_dir = TIMER_COUNT_DOWN;
    timer_config.counter_en = TIMER_PAUSE;
    timer_config.alarm_en = TIMER_ALARM_EN;
    timer_config.auto_reload = TIMER_AUTORELOAD_DIS;

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ZERO_DETECT_IO, gpio_isr_handler, (void*) ZERO_DETECT_IO);

    timer_init(TIMER_GROUP_0, TIMER_IDX_0, &timer_config);
    // kick_off_timer(HALF_WAVE_PERIOD_MS);


    printf("INITIAL_TIMER_TICKS: %u\n", INITIAL_TIMER_TICKS);

    while(1) {
      printf("%d delay %d zeros: %d total zeros: %d timer_isr_counter: %u timer_isr_cleared: %u\n"
        , cnt ++
        , triac_off_time
        , zero_cross_count
        , total_zero_cross
        , timer_isr_counter
        , timer_isr_cleared
      );
      zero_cross_count = 0;

      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
