/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
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

void gpio_setup(const gpio_num_t pin) {

  struct pir_frame_t {
    uint16_t channel[3];
  };

  struct pir_frame_t frame = {0};

  int channel, bit;

  gpio_config_t c = {0};
  c.pin_bit_mask = 1 << pin;
  c.mode = GPIO_MODE_OUTPUT; // start here to reset to 0
  c.pull_down_en = GPIO_PULLDOWN_ENABLE;
  c.pull_up_en = GPIO_PULLUP_DISABLE;
  c.intr_type = GPIO_PIN_INTR_DISABLE;

  gpio_config(&c);
  gpio_set_level(pin, 0);

/*
  if (ESP_OK != gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT)) {
    printf("gpio_set_direction FAILED\n");
    return;
  }
  if (ESP_OK != gpio_set_pull_mode(pin, GPIO_FLOATING)) {
    printf("gpio_set_pull_mode FAILED\n");
    return;
  }
  if (ESP_OK != gpio_set_intr_type(pin, GPIO_INTR_DISABLE)) {
    printf("gpio_set_intr_type FAILED\n");
    return;
  }
  printf("gpio setup ok\n");
  */

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

    printf("ch0: %d\tch1: %d\ttmp: %d\n", frame.channel[0], frame.channel[1], frame.channel[2]);

    // take a breath between packets
    vTaskDelay(100 / portTICK_PERIOD_MS);
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

void app_main()
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("ets_get_cpu_frequency: %u\n", ets_get_cpu_frequency());

    xTaskCreate(task_pir, "PIR_TIMER", 2048, NULL, 10, NULL);

    while (1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
/*
    for (int i = 100; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    */
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
