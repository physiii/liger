#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "xtensa/corebits.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "soc/cpu.h"

#include <stdio.h>
#include "esp_types.h"
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#define PIR_FREQ_KHZ (32)
#define PIR_PERIOD_US ((uint64_t)(1000000.0 / (PIR_FREQ_KHZ * 1000)))
#define PIR_IO (GPIO_NUM_15)
#define PIR_BITS (42)
#define CPU_FREQ_MHZ (240)
#define CPU_TICK_US (1000000.0 / (CPU_FREQ_MHZ * 1000000))
#define OUTPUT_MODE (1)
#define INPUT_MODE (0)

#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define TIMER_INTERVAL0_SEC   (3.4179) // sample test interval for the first timer
#define TIMER_INTERVAL1_SEC   (5.78)   // sample test interval for the second timer
#define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
#define TEST_WITH_RELOAD 1 // testing will be done with auto reload

#define PIR_TIMER_INTERVAL   (0.015625) // pir timer to get 64 Hz

int motion_state = 0;
uint16_t motion_level;
uint16_t motion_tmp;
double alpha = 0.8;
double alpha_avg = 0.001;
int pir_threshold_high = 180;
int pir_threshold_low = 120;
int motion_threshold = 2000;
int motion_count_threshold = 4;
int motion_count = 0;
int motion_duration = 0;
int delta_diff = 0;
int buffer_size = 25;
int buffer[25]; // use buffer size
int warmup_time = 400;
//int bandpass_low = 16200;
//int bandpass_high = 16800;
int bandpass_low = 1000;
int bandpass_high = 15000;
float delta_bandpass_low = 0.535;
float delta_bandpass_high = 0.9;
int pir_bits_remaining; // set when timer starts, decremented in handler
uint64_t pir_bits; // 0-41 used; 42-63 unused
uint64_t t0, t1;
int debounce_delay = 0;
int debounce_count = 0;
bool debounce = false;
bool data_tx_started = false;
int channel, bit;
int min = 99999;
int max = 0;
int accumulator = 8000;
int accumulator_avg = 8000;
int accumulator_1 = 8000;
int accumulator_1_avg = 8000;
int difference = 0;
int difference_1 = 0;
int warmup = 0;
int count = 0;
int pir_timer_count = 0;
int motion_value = 0;
int motion_value_1 = 0;

void debounce_pir(int delay) {
  debounce_delay = delay / PIR_TIMER_INTERVAL;
  debounce_count = 0;
}

struct pir_frame_t {
  uint16_t channel[3];
};

struct pir_frame_t frame = {0};
struct pir_frame_t previous_frame = {0};
struct pir_frame_t delta_frame = {0};
struct pir_frame_t previous_delta_frame = {0};
struct pir_frame_t accumulator_frame = {0};
struct pir_frame_t average_frame = {0};

// best observed freq has been 20KHz
void cb_pir_timer(void * arg) {
  pir_bits_remaining -= 1;
  // printf("tick; remaining: %d\n", pir_bits_remaining);
  if (pir_bits_remaining <= 0) {
    t1 = esp_timer_get_time();
    esp_timer_stop(*(esp_timer_handle_t*)arg);
    esp_timer_delete(*(esp_timer_handle_t*)arg);
    printf("stopped and deleted timer\n");

    printf("t0: %llu, t1: %llu, diff: %llu\n", t0, t1, t1 - t0);
  }
}

inline void busy_delay_us(float us) {
  uint32_t t0_ccount, t1_ccount;
  t0_ccount = CCOUNT;
  while (true) {
    RSR(CCOUNT, t1_ccount);
    if (t1_ccount - t0_ccount > ((uint32_t)(us / CPU_TICK_US))) {
      break;
    }
  }
}

bool get_motion_state() {
  int state = motion_state;
  motion_state = false;
  return state;
}

void zero_array (int array[],int size) {
  for (int i = 0;i < size;i++) {
    array[i] = 0;
    //printf("i\t%d\n",i);
  }
}

int find_min_max(int arr[], int size, int * min, int * max) {
  *max = arr[0];
  *min = arr[0];

  for(int i=1; i<size; i++) {
    if(arr[i] > *max) *max = arr[i];
    if(arr[i] < *min) *min = arr[i];
  }

  //printf("min: %d\tmax: %d\n", min, max);
  return 0;
}

int count_motion_events(int arr[],int size) {
  int total = 0;
  for(int i=0; i<size; i++) {
    int diff = arr[i] - arr[i-1];
    if (diff > 0) total+=diff;
    if (diff < 0) total-=diff;
  }
  return total;
}

int sum_diff_between_elements(int arr[],int size) {
  int total = 0;
  for(int i=1; i<size; i++) {
    //average += (arr[i] - arr[i-1])*(arr[i] - arr[i-1]);
    int diff = arr[i] - arr[i-1];
    if (diff > 0) total+=diff;
    if (diff < 0) total-=diff;
  }
  return total;
}

int check_for_motion(int buffer[],int size) {
  // default to no motion
  int state = 0;

  // get differences between adjacent elements and total them
  int sum_diff = sum_diff_between_elements(buffer, buffer_size);
  if (sum_diff) printf("sum_diff: %d\n",sum_diff);

  if (sum_diff > motion_threshold) {
    state = 1;
  }

  return state;
}

void set_gpio_mode (int mode) {

  if (mode == OUTPUT_MODE) {
    gpio_config_t c = {0};
    c.pin_bit_mask = 1 << PIR_IO;
    c.mode = GPIO_MODE_OUTPUT; // start here to reset to 0
    c.pull_down_en = GPIO_PULLDOWN_ENABLE;
    c.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&c);
  }

  if (mode == INPUT_MODE) {
    gpio_config_t c = {0};
    c.pin_bit_mask = 1 << PIR_IO;
    c.mode = GPIO_MODE_INPUT; // start here to reset to 0
    c.pull_down_en = GPIO_PULLDOWN_ENABLE;
    c.pull_up_en = GPIO_PULLUP_DISABLE;
    //c.intr_type = GPIO_PIN_INTR_POSEDGE;
    gpio_config(&c);
  }

}

void pir_loop() {

  struct pir_frame_t {
    uint16_t channel[3];
  };

  int channel, bit;
  int min = 99999;
  int max = 0;
  int accumulator = 16000;
  int accumulator_avg = 16000;
  int difference = 0;
  int warmup = 0;
  int buffer[buffer_size];
  int count = 0;

  zero_array(&buffer,buffer_size);

  while (1) {

    // wait DL high
    gpio_set_direction(PIR_IO, GPIO_MODE_INPUT);
    while (0 == gpio_get_level(PIR_IO));

    // wait > 25us
    busy_delay_us(25.0);

    for (channel = 0; channel < 3; channel += 1) {
      frame.channel[channel] = 0;
      for (bit = 0; bit < 14; bit += 1) {
        gpio_set_direction(PIR_IO, GPIO_MODE_OUTPUT);
        // busy_delay_us(0.2);
        gpio_set_level(PIR_IO, 0);
        busy_delay_us(0.2);
        gpio_set_level(PIR_IO, 1);
        busy_delay_us(0.2);

        frame.channel[channel] <<= 1;
        gpio_set_direction(PIR_IO, GPIO_MODE_INPUT);
        // busy_delay_us(0.1);
        if (gpio_get_level(PIR_IO)) {
          frame.channel[channel] |= 1;
        }
      }
    }


    // bandpass filter on raw data
    // if (frame.channel[0] > bandpass_low && frame.channel[0] < bandpass_high) {
    //
    //   // exponential moving average
    //   // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
    //   // The closer alpha is to zero the more older values will contribute.
    //   accumulator = (alpha * frame.channel[0]) + (1.0 - alpha) * accumulator;
    //   accumulator_avg = (alpha_avg * frame.channel[0]) + (1.0 - alpha_avg) * accumulator_avg;
    //
    //   //subtract background "average" and fill buffer
    //   difference = accumulator - accumulator_avg;
    //   buffer[count] = difference;
    //
    //   //check for motion when buffer is full
    //   if (count < buffer_size) {
    //     count++;
    //   } else {
    //     motion_state = check_for_motion(buffer,buffer_size);
    //     (motion_state) ? (motion_count++) : (motion_count=0);
    //
    //     //set motion state to zero if debounce activated
    //     if (debounce_count < debounce_delay) motion_state = 0;
    //
    //     //set motion state to zero if motion count threshold is not met
    //     if (motion_count < motion_count_threshold) motion_state = 0;
    //     count = 0;
    //   }
    // }
    // debounce_count++;
    //previous_delta_frame = delta_frame;
    //previous_frame = frame;
    // take a breath between packets
    vTaskDelay(15 / portTICK_PERIOD_MS);
  }
}

void fill_pir_frame() {

  // wait DL high
  gpio_set_direction(PIR_IO, GPIO_MODE_INPUT);
  while (0 == gpio_get_level(PIR_IO));

  // wait 25us for setup
  busy_delay_us(25.0);

  for (channel = 0; channel < 3; channel++) {
    frame.channel[channel] = 0;
    for (bit = 0; bit < 14; bit += 1) {

      //pir expects low to high transition to get each bit
      gpio_set_direction(PIR_IO, GPIO_MODE_OUTPUT);
      gpio_set_level(PIR_IO, 0);
      busy_delay_us(0.2);
      gpio_set_level(PIR_IO, 1);
      busy_delay_us(0.2);

      // shift frame by one for next bit
      frame.channel[channel] <<= 1;
      gpio_set_direction(PIR_IO, GPIO_MODE_INPUT);

      // wait 5us for bit to settle
      busy_delay_us(5);

      // read bit and put into frame
      if (gpio_get_level(PIR_IO)) frame.channel[channel] |= 1;
    }
  }

  if (frame.channel[0] > bandpass_low && frame.channel[0] < bandpass_high) {
    motion_value += frame.channel[0];
    accumulator = (alpha * frame.channel[0]) + (1.0 - alpha) * accumulator;
    accumulator_avg = (alpha_avg * frame.channel[0]) + (1.0 - alpha_avg) * accumulator_avg;
    difference = accumulator - accumulator_avg;
    if (difference < 0) difference= 0 - difference;
    if (difference > motion_threshold) motion_state = 1;
  }

  if (frame.channel[1] > bandpass_low && frame.channel[1] < bandpass_high) {
    motion_value_1 += frame.channel[1];
    accumulator_1 = (alpha * frame.channel[1]) + (1.0 - alpha) * accumulator_1;
    accumulator_1_avg = (alpha_avg * frame.channel[1]) + (1.0 - alpha_avg) * accumulator_1_avg;
    difference_1 = accumulator_1 - accumulator_1_avg;
    if (difference_1 < 0) difference_1= 0 - difference_1;
    //if (difference_1 > motion_threshold) motion_state = 1;
  }

  // motion_state = check_for_motion(buffer,buffer_size);
  // (motion_state) ? (motion_count++) : (motion_count=0);

  // set motion state to zero if debounce activated
  if (debounce_count < debounce_delay) motion_state = 0;
  debounce_count++;

  // set motion state to zero if motion count threshold is not met
  // if (motion_count < motion_count_threshold) motion_state = 0;
  // count = 0;

/*
  // bandpass filter on raw data
  if (frame.channel[0] > bandpass_low && frame.channel[0] < bandpass_high) {

    // exponential moving average
    // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
    // The closer alpha is to zero the more older values will contribute.
    accumulator = (alpha * frame.channel[0]) + (1.0 - alpha) * accumulator;
    accumulator_avg = (alpha_avg * frame.channel[0]) + (1.0 - alpha_avg) * accumulator_avg;

    //subtract background "average" and fill buffer
    difference = accumulator -accumulator_avg;
    buffer[count] = difference;

    //check for motion when buffer is full
    if (count < buffer_size) {
      count++;
    } else {
      motion_state = check_for_motion(buffer,buffer_size);
      (motion_state) ? (motion_count++) : (motion_count=0);

      //set motion state to zero if debounce activated
      if (debounce_count < debounce_delay) motion_state = 0;

      //set motion state to zero if motion count threshold is not met
      if (motion_count < motion_count_threshold) motion_state = 0;
      count = 0;
    }
  }
*/
}

static void IRAM_ATTR pir_isr_handler(void* arg) {

    //pir starts by pulling pin high
    if (gpio_get_level(PIR_IO)) {
      data_tx_started = true;

      // wait 25us for setup
      busy_delay_us(25.0);

      // read bits from pir
      fill_pir_frame();
    }
}

xQueueHandle timer_queue;

/*
 * Timer group0 ISR handler
 *
 * Note:
 * We don't call the timer API here because they are not declared with IRAM_ATTR.
 * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
 * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
 */
void IRAM_ATTR timer_group1_isr(void *para)
{
    int timer_idx = (int) para;

    /* Retrieve the interrupt status and the counter value
       from the timer that reported the interrupt */
    uint32_t intr_status = TIMERG1.int_st_timers.val;
    TIMERG1.hw_timer[timer_idx].update = 1;
    uint64_t timer_counter_value =
        ((uint64_t) TIMERG1.hw_timer[timer_idx].cnt_high) << 32
        | TIMERG1.hw_timer[timer_idx].cnt_low;

    /* Prepare basic event data
       that will be then sent back to the main program task */
    timer_event_t evt;
    evt.timer_group = 0;
    evt.timer_idx = timer_idx;
    evt.timer_counter_value = timer_counter_value;

    /* Clear the interrupt
       and update the alarm time for the timer with without reload */
    if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
        evt.type = TEST_WITHOUT_RELOAD;
        TIMERG1.int_clr_timers.t0 = 1;
        timer_counter_value += (uint64_t) (TIMER_INTERVAL0_SEC * TIMER_SCALE);
        TIMERG1.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
        TIMERG1.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
    } else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
        evt.type = TEST_WITH_RELOAD;
        TIMERG1.int_clr_timers.t1 = 1;
    } else {
        evt.type = -1; // not supported even type
    }

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    TIMERG1.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

    fill_pir_frame();
    pir_timer_count++;
}

/*
 * Initialize selected timer of the timer group 0
 *
 * timer_idx - the timer number to initialize
 * auto_reload - should the timer auto reload on alarm?
 * timer_interval_sec - the interval of alarm to set
 */
static void example_tg1_timer_init(int timer_idx,
    bool auto_reload, double timer_interval_sec)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    timer_init(TIMER_GROUP_1, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_1, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_1, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_1, timer_idx);
    timer_isr_register(TIMER_GROUP_1, timer_idx, timer_group1_isr,
        (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_1, timer_idx);
}

void task_pir(void * param) {
    esp_timer_handle_t t = NULL;
    esp_err_t result;

    printf("PIR freq: %d KHz, %lluus Period\n", PIR_FREQ_KHZ, PIR_PERIOD_US);

    gpio_config_t c = {0};
    c.pin_bit_mask = 1 << PIR_IO;
    c.mode = GPIO_MODE_OUTPUT; // start here to reset to 0
    c.pull_down_en = GPIO_PULLDOWN_ENABLE;
    c.pull_up_en = GPIO_PULLUP_DISABLE;
    c.intr_type = GPIO_PIN_INTR_DISABLE;
    gpio_config(&c);

    gpio_set_level(PIR_IO, 0);

    gpio_isr_handler_add(PIR_IO, pir_isr_handler, (void*) PIR_IO);

    zero_array(&buffer,buffer_size);


    while (1) {
      // if (!data_tx_started) {
      //   //pir starts by pulling pin high
      //   if (gpio_get_level(PIR_IO)) {
      //     data_tx_started = true;
      //
      //     // wait 25us for setup
      //     busy_delay_us(25.0);
      //
      //     // read bits from pir
      //     fill_pir_frame();
      //   }
      // }

      vTaskDelay(1000 / portTICK_PERIOD_MS);
      printf("ch0\t%d\tch1\t%d\n",difference,difference_1);
      pir_timer_count = 0;
      motion_value = 0;

      // timer_event_t evt;
      // xQueueReceive(timer_queue, &evt, portMAX_DELAY);
      //
      // /* Print information that the timer reported an event */
      // if (evt.type == TEST_WITHOUT_RELOAD) {
      //     printf("\n    Example timer without reload\n");
      // } else if (evt.type == TEST_WITH_RELOAD) {
      //     printf("\n    Example timer with auto reload\n");
      // } else {
      //     printf("\n    UNKNOWN EVENT TYPE\n");
      // }
      // printf("Group[%d], timer[%d] alarm event\n", evt.timer_group, evt.timer_idx);
      //
      // /* Print the timer values passed by event */
      // printf("------- EVENT TIME --------\n");
      // print_timer_counter(evt.timer_counter_value);
      //
      // /* Print the timer values as visible by this task */
      // printf("-------- TASK TIME --------\n");
      // uint64_t task_counter_value;
      // timer_get_counter_value(evt.timer_group, evt.timer_idx, &task_counter_value);
      // print_timer_counter(task_counter_value);

    }
    //pir_loop();
}

void pir_main() {
    example_tg1_timer_init(TIMER_1, TEST_WITH_RELOAD, PIR_TIMER_INTERVAL);
    xTaskCreate(task_pir, "PIR_TIMER", 2048, NULL, 10, NULL);
}
