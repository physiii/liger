/* I2S Example

    This example code will output 100Hz sine wave and triangle wave to 2-channel of I2S driver
    Every 5 seconds, it will change bits_per_sample [16, 24, 32] for i2s data

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include <math.h>


#define SAMPLE_RATE     (36000)
#define I2S_NUM         (0)
#define WAVE_FREQ_HZ    (100)
#define PI 3.14159265

#define I2S_NUM_RX         (1)
#define bitDepth        (32)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)

static void setup_triangle_sine_waves(int bits)
{
    int *samples_data = malloc(((bits+8)/16)*SAMPLE_PER_CYCLE*4);
    unsigned int i, sample_val;
    double sin_float, triangle_float, triangle_step = (double) pow(2, bits) / SAMPLE_PER_CYCLE;
    size_t i2s_bytes_write = 0;

    printf("\r\nTest bits=%d free mem=%d, written data=%d\n", bits, esp_get_free_heap_size(), ((bits+8)/16)*SAMPLE_PER_CYCLE*4);

    triangle_float = -(pow(2, bits)/2 - 1);

    for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
        sin_float = sin(i * PI / 180.0);
        if(sin_float >= 0)
            triangle_float += triangle_step;
        else
            triangle_float -= triangle_step;

        sin_float *= (pow(2, bits)/2 - 1);

        if (bits == 16) {
            sample_val = 0;
            sample_val += (short)triangle_float;
            sample_val = sample_val << 16;
            sample_val += (short) sin_float;
            samples_data[i] = sample_val;
        } else if (bits == 24) { //1-bytes unused
            samples_data[i*2] = ((int) triangle_float) << 8;
            samples_data[i*2 + 1] = ((int) sin_float) << 8;
        } else {
            samples_data[i*2] = ((int) triangle_float);
            samples_data[i*2 + 1] = ((int) sin_float);
        }

    }

    i2s_set_clk(I2S_NUM, SAMPLE_RATE, bits, 2);
    //Using push
    // for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
    //     if (bits == 16)
    //         i2s_push_sample(0, &samples_data[i], 100);
    //     else
    //         i2s_push_sample(0, &samples_data[i*2], 100);
    // }
    // or write
    i2s_write(I2S_NUM, samples_data, ((bits+8)/16)*SAMPLE_PER_CYCLE*4, &i2s_bytes_write, 100);

    free(samples_data);
}
void app_main()
{


  i2s_config_t i2s_config_rx = {
    .mode = I2S_MODE_MASTER | I2S_MODE_RX,
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = bitDepth,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .dma_buf_count = 32,
    .dma_buf_len = 64,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1
};
i2s_pin_config_t pin_config_rx = {
    .bck_io_num = 26,
    .ws_io_num = 23,
    .data_out_num = -1,
    .data_in_num = 22
};

gpio_set_direction(GPIO_NUM_22, GPIO_MODE_INPUT);
gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT);
gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);

i2s_driver_install(I2S_NUM_RX, &i2s_config_rx, 0, NULL);
i2s_set_pin(I2S_NUM_RX, &pin_config_rx);

    //for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample)
    //depend on bits_per_sample
    //using 6 buffers, we need 60-samples per buffer
    //if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes
    //if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes
    /*i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,                                  // Only TX
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
        .use_apll = false,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1                                //Interrupt level 1
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 23,
        .data_out_num = 25,
        .data_in_num = 22                                                       //Not used
    };
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);*/

    int test_bits = 16;
    bool startup = true;

    uint16_t buf_len = 1024;
    char *buf = calloc(buf_len, sizeof(char));

    while (1) {
      /*if (startup) {
        startup = false;
        setup_triangle_sine_waves(test_bits);
      } else {
        setup_triangle_sine_waves(0);
      }*/

      size_t bytes_read = 0;
      while (bytes_read == 0) {
        i2s_read(I2S_NUM_RX, buf, (size_t) buf_len, &bytes_read, 0);
      }
      int samples_read = bytes_read / 2 / (bitDepth / 8);

      printf("samples read %d | buff %s\n",samples_read,buf);
      vTaskDelay(1000/portTICK_RATE_MS);
        /*test_bits += 8;
        if(test_bits > 32)
            test_bits = 16;*/

    }

}
