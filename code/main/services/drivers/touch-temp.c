/* Touch Pad Read Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_FILTER_MODE_EN  (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

#define UP_PAD 7
#define DOWN_PAD 5
#define LEFT_PAD 9
#define RIGHT_PAD 8

int touch_threshold = 1100;
uint16_t touch_value;
uint16_t touch_filter_value;

static void tp_example_touch_pad_init()
{
  touch_pad_config(UP_PAD, TOUCH_THRESH_NO_USE);
  touch_pad_config(DOWN_PAD, TOUCH_THRESH_NO_USE);
  touch_pad_config(LEFT_PAD, TOUCH_THRESH_NO_USE);
  touch_pad_config(RIGHT_PAD, TOUCH_THRESH_NO_USE);
}

int get_pad_state(int pad) {
  touch_pad_read_raw_data(pad, &touch_value);
  if (touch_value > touch_threshold) {
    return 0;
  } else return 1;
}

int get_dpad_state() {

  if (get_pad_state(UP_PAD)
    && get_pad_state(DOWN_PAD)
    && get_pad_state(LEFT_PAD)
    && get_pad_state(RIGHT_PAD))
    return 1;

  if (get_pad_state(UP_PAD) && get_pad_state(RIGHT_PAD))
    return 2;

  if (get_pad_state(RIGHT_PAD) && get_pad_state(DOWN_PAD))
    return 3;

  if (get_pad_state(DOWN_PAD) && get_pad_state(LEFT_PAD))
    return 4;

  if (get_pad_state(LEFT_PAD) && get_pad_state(UP_PAD))
    return 5;

  if (get_pad_state(UP_PAD)) return 6;
  if (get_pad_state(RIGHT_PAD)) return 7;
  if (get_pad_state(DOWN_PAD)) return 8;
  if (get_pad_state(LEFT_PAD)) return 9;

  char buff[512];
  return 0;
}

void touch_main()
{
    // Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    touch_pad_init();
    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    tp_example_touch_pad_init();
#if TOUCH_FILTER_MODE_EN
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
#endif

#if TOUCH_FILTER_MODE_EN
  printf("Touch Sensor filter mode read, the output format is: \nTouchpad num:[raw data, filtered data]\n\n");
#else
  printf("Touch Sensor normal mode read, the output format is: \nTouchpad num:[raw data]\n\n");
#endif
    // Start task to read values sensed by pads
    //xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);
}
