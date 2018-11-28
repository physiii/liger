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

bool motion_active = false;
uint16_t motion_data_0;
uint16_t motion_data_1;
uint16_t motion_tmp;

float alpha = 0.01;
bool pir_debounce = false;

int threshold_low = 15000;
int threshold_high = 17000;
int delta_threshold_low = 7000;
int delta_threshold_high = 9000;

int pir_bits_remaining; // set when timer starts, decremented in handler
uint64_t pir_bits; // 0-41 used; 42-63 unused
uint64_t t0, t1;

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

void gpio_setup(const gpio_num_t pin) {

  struct pir_frame_t {
    uint16_t channel[3];
  };

  struct pir_frame_t frame = {0};
  struct pir_frame_t previous_frame = {0};
  struct pir_frame_t delta_frame = {0};
  struct pir_frame_t previous_delta_frame = {0};
  struct pir_frame_t accumulator_frame = {0};

  int channel, bit;

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
    // The closer alpha is to one the more older values will contribute.
    accumulator_frame.channel[0] = (alpha * frame.channel[0]) + (1.0 - alpha) * accumulator_frame.channel[0];
    accumulator_frame.channel[1] = (alpha * frame.channel[1]) + (1.0 - alpha) * accumulator_frame.channel[1];
    accumulator_frame.channel[2] = (alpha * frame.channel[2]) + (1.0 - alpha) * accumulator_frame.channel[2];
    //printf("pir: %d %d\n",accumulator_frame.channel[0],accumulator_frame.channel[1]);
    if (accumulator_frame.channel[0] < threshold_low || accumulator_frame.channel[0] > threshold_high){
      printf("ch0: %d\tch1: %d\ttmp: %d\n", accumulator_frame.channel[0], accumulator_frame.channel[1], accumulator_frame.channel[2]);
      motion_active = true;
      motion_data_0 = accumulator_frame.channel[0];
      motion_data_1 = accumulator_frame.channel[1];
      motion_tmp = accumulator_frame.channel[1];
      pir_debounce = true;
    }


    delta_frame.channel[0] = accumulator_frame.channel[0] - previous_frame.channel[0];
    delta_frame.channel[1] = accumulator_frame.channel[1] - previous_frame.channel[1];
    delta_frame.channel[2] = accumulator_frame.channel[2] - previous_frame.channel[2];

    if (delta_frame.channel[0] > delta_threshold_low && delta_frame.channel[0] < delta_threshold_high) {
      printf("previous delta: %d\n",previous_frame.channel[0]);
      printf("delta: %d\n",delta_frame.channel[0]);
      printf("delta delta: %d\n",previous_delta_frame.channel[0] - delta_frame.channel[0]);
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

    esp_timer_create_args_t timer_create_args = {};
    timer_create_args.callback = cb_pir_timer;
    timer_create_args.dispatch_method = ESP_TIMER_TASK;
    timer_create_args.arg = &t;
    timer_create_args.name = "PIR";
    result = esp_timer_create(&timer_create_args, &t);
    switch (result) {
      case ESP_OK:
        break;
      case ESP_ERR_INVALID_ARG:
        printf("ESP_ERR_INVALID_ARG\n");
        break;
      case ESP_ERR_INVALID_STATE:
        printf("ESP_ERR_INVALID_STATE\n");
        break;
      case ESP_ERR_NO_MEM:
       printf("ESP_ERR_NO_MEM\n");
        break;
      default:
        printf("esp_timer_create WTF");
    }

    if (result == ESP_OK) {
      pir_bits_remaining = PIR_BITS;
      t0 = esp_timer_get_time();
      result = esp_timer_start_periodic(t, PIR_PERIOD_US);
      switch (result) {
        case ESP_OK:
          printf("timer started\n");
          break;
        case ESP_ERR_INVALID_ARG:
          printf("ESP_ERR_INVALID_ARG\n");
          break;
        case ESP_ERR_INVALID_STATE:
          printf("ESP_ERR_INVALID_STATE\n");
          break;
        default:
          printf("esp_timer_start_periodic WTF");
      }
    }

    while (1) {
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void pir_main() {
    xTaskCreate(task_pir, "PIR_TIMER", 2048, NULL, 10, NULL);
    //printf("Restarting now.\n");
    //fflush(stdout);
    //esp_restart();
}
