#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "xtensa/corebits.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "soc/cpu.h"

#define SIREN_IO    2
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<SIREN_IO))
#define SIREN_OFF 0
#define SIREN_ON 1

int chirp_delay = 100;

int
setSiren(int level)
{
  if (level) {
    gpio_set_level(SIREN_IO, SIREN_ON);
  } else {
    gpio_set_level(SIREN_IO, SIREN_OFF);
  }
  return level;
}

int
chirpSiren(int number_of_chirps)
{
  lwsl_notice("chirp siren: %d\n",number_of_chirps);
  for (int i=0; i < number_of_chirps;i++){
    setSiren(1);
    vTaskDelay(chirp_delay / portTICK_PERIOD_MS);
    setSiren(0);
    vTaskDelay(chirp_delay / portTICK_PERIOD_MS);
  }
  return 1;
}


void siren_driver_main()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    printf("starting siren driver\n");
}
