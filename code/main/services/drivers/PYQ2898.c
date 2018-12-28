#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "xtensa/corebits.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "soc/cpu.h"

#define PIR_FREQ_KHZ (32)
#define PIR_PERIOD_US ((uint64_t)(1000000.0 / (PIR_FREQ_KHZ * 1000)))
#define PIR_DL_GPIO (GPIO_NUM_15)
#define PIR_BITS (42)
#define CPU_FREQ_MHZ (240)
#define CPU_TICK_US (1000000.0 / (CPU_FREQ_MHZ * 1000000))

int debounce_ms = 5 * 1000;
bool motion_active = false;
uint16_t motion_level;
uint16_t motion_tmp;
float alpha = 0.1;
float alpha_avg = 0.005;
int pir_threshold_high = 180;
int pir_threshold_low = 120;
int motion_threshold = 65;
int motion_duration_thresh = 10;
int motion_count = 0;
int motion_duration = 0;
int delta_diff = 0;
int delta_diff_count = 30;
int warmup_time = 400;
int pir_lowpass = 16200;
int pir_highpass = 16800;
int debounce_duration = 400;
float delta_pir_lowpass = 0.535;
float delta_pir_highpass = 0.9;
int pir_bits_remaining; // set when timer starts, decremented in handler
uint64_t pir_bits; // 0-41 used; 42-63 unused
uint64_t t0, t1;
int debounce_cnt = 0;

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
  bool state = motion_active;
  motion_active = false;
  return state;
}

void debounce_pir () {
  debounce_cnt = 0;
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

int avg_diff_between_elements(int arr[],int size) {
  int average = 0;
  for(int i=1; i<size; i++) {
    //average += (arr[i] - arr[i-1])*(arr[i] - arr[i-1]);
    int diff = arr[i] - arr[i-1];
    if (diff > 0) average+=diff;
    if (diff < 0) average-=diff;
  }
  //average = average / size;
  return average;
}

void gpio_setup(const gpio_num_t pin) {

  struct pir_frame_t {
    uint16_t channel[3];
  };

  struct pir_frame_t frame = {0};
  struct pir_frame_t previous_frame = {0};
  struct pir_frame_t delta_frame = {0};
  struct pir_frame_t previous_delta_frame = {0};
  struct pir_frame_t accumulator_frame = {0};
  struct pir_frame_t average_frame = {0};

  int channel, bit;
  int min = 99999;
  int max = 0;
  int accumulator = 16000;
  int accumulator_avg = 16000;
  int difference = 0;
  int warmup = 0;
  int difference_arr[delta_diff_count];
  int count = 0;


  zero_array(&difference_arr,delta_diff_count);

  gpio_config_t c = {0};
  c.pin_bit_mask = 1 << pin;
  c.mode = GPIO_MODE_OUTPUT; // start here to reset to 0
  c.pull_down_en = GPIO_PULLDOWN_ENABLE;
  c.pull_up_en = GPIO_PULLUP_DISABLE;
  c.intr_type = GPIO_PIN_INTR_DISABLE;

  gpio_config(&c);
  gpio_set_level(pin, 0);

  while (1) {

    uint64_t pir_frame = 0;

    // wait DL high
    // printf("waiting for dl\n");
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    while (0 == gpio_get_level(pin));
    // printf("waited for dl\n");

    // wait > 25us
    busy_delay_us(25.0);
    // printf("waited 25us\n");

    for (channel = 0; channel < 3; channel += 1) {
      frame.channel[channel] = 0;
      for (bit = 0; bit < 14; bit += 1) {
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        // busy_delay_us(0.2);
        gpio_set_level(pin, 0);
        busy_delay_us(0.2);
        gpio_set_level(pin, 1);
        busy_delay_us(0.2);

        frame.channel[channel] <<= 1;
        gpio_set_direction(pin, GPIO_MODE_INPUT);
        // busy_delay_us(0.1);
        if (gpio_get_level(pin)) {
          frame.channel[channel] |= 1;
        }
      }
    }

    // exponential moving average
    // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
    // The closer alpha is to zero the more older values will contribute.

    if (frame.channel[0] > pir_lowpass && frame.channel[0] < pir_highpass) {

      accumulator = (alpha * frame.channel[0]) + (1.0 - alpha) * accumulator;
      accumulator_avg = (alpha_avg * frame.channel[0]) + (1.0 - alpha_avg) * accumulator_avg;
      // difference = frame.channel[0] - accumulator;
      difference = accumulator - accumulator_avg;
      (count < delta_diff_count) ? (count++) : (count=0);
      difference_arr[count] = difference;
      //find_min_max(difference_arr, delta_diff_count, &min, &max);
      int avg_diff = avg_diff_between_elements(difference_arr, delta_diff_count);
      //delta_diff = max - min;
      if (count == 0) printf("avg_diff: %d\n", avg_diff);

      //printf("pir: %d\twarmup: %d\tdebounce: %d\n",difference,warmup,debounce_cnt);
      //printf("diff: %d\taccumulator: %d\taccumulator_avg: %d\n",difference,accumulator,accumulator_avg);


      //if ((difference >= pir_threshold_high || difference <= pir_threshold_low) && warmup > warmup_time && debounce_cnt > debounce_duration){
      if (avg_diff>motion_threshold
        && warmup > warmup_time
        && debounce_cnt > debounce_duration){
        motion_count++;
        if (motion_count >= motion_duration_thresh) {
          printf("motion: %d\n",motion_threshold);
          //printf("ch0: %d\tch1: %d\ttmp: %d\n", accumulator_frame.channel[0], accumulator_frame.channel[1], accumulator_frame.channel[2]);
          motion_active = true;
          motion_level = avg_diff;
          motion_duration = motion_count;
          motion_tmp = accumulator_frame.channel[1];
          debounce_cnt = 0;
        }
      } else {
        motion_count = 0;
      }
      debounce_cnt++;
      warmup++;
    }

    previous_delta_frame = delta_frame;
    previous_frame = frame;
    // take a breath between packets
    vTaskDelay(15 / portTICK_PERIOD_MS);
  }
}

void task_pir(void * param) {
    esp_timer_handle_t t = NULL;
    esp_err_t result;

    printf("PIR freq: %d KHz, %lluus Period\n", PIR_FREQ_KHZ, PIR_PERIOD_US);

    gpio_setup(PIR_DL_GPIO);
}

void pir_main() {
    xTaskCreate(task_pir, "PIR_TIMER", 2048, NULL, 10, NULL);
}
