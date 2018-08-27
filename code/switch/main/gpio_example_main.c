#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "driver/ledc.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "soc/cpu.h"

#define TRIAC_IO    2
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<TRIAC_IO))
#define ZERO_DETECT_IO     13
#define GPIO_INPUT_PIN_SEL  ((1ULL<<ZERO_DETECT_IO))
#define ESP_INTR_FLAG_DEFAULT 0
#define TRIAC_OFF 0
#define TRIAC_ON 1

#define AC_FREQ_HZ (120)

#define PWM_DUTY_RESOLUTION (LEDC_TIMER_19_BIT)

#define FADE_DURATION_MS (3000)
#define FADE_UP_TARGET (1.0f)
#define FADE_DOWN_TARGET (0.0f)

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

int total_zero_cross = 0;
int fade_direction = 1;
int fade_loops = 0;
static void IRAM_ATTR gpio_isr_handler(void *arg) {
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
    uint32_t io_num;
    gpio_set_level(TRIAC_IO, TRIAC_OFF);

    for(;;) {

      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    //gpio_set_intr_type(ZERO_DETECT_IO, GPIO_INTR_ANYEDGE);
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0);
    // ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, ledc_channel.duty);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0); // (uint32_t)((1 << PWM_DUTY_RESOLUTION) * ((fade_direction == 1) ? FADE_UP_TARGET : FADE_DOWN_TARGET)));
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ZERO_DETECT_IO, gpio_isr_handler, (void*) ZERO_DETECT_IO);

    while(1) {
      printf("%d delay %d zeros: %d total zeros: %d fades: %d duty: %u\n"
        , cnt ++
        , triac_off_time
        , zero_cross_count
        , total_zero_cross
        , fade_loops
        , ledc_get_duty(ledc_channel.speed_mode, ledc_channel.channel)
      );
      zero_cross_count = 0;

      vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
