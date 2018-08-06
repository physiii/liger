#include "drivers/touch.c"

char buttons_service_message[2000];
bool buttons_service_message_ready = false;
static void buttons_service(void *pvParameter)
{
  uint16_t touch_value;
  uint16_t touch_filter_value;
  touch_main();

#if TOUCH_FILTER_MODE_EN
  printf("Touch Sensor filter mode read, the output format is: \nTouchpad num:[raw data, filtered data]\n\n");
#else
  printf("Touch Sensor normal mode read, the output format is: \nTouchpad num:[raw data]\n\n");
#endif
  while (1) {
      strcpy(buttons_service_message,"{\"event_type\":\"buttons\",\"payload\":[");
      char buff[512];
      int touch_val = 0;
      for (int i = 0; i < TOUCH_PAD_MAX; i++) {
          if (i <= 4) continue;
          if (i == 6) continue;
          strcpy(buff,"");
#if TOUCH_FILTER_MODE_EN
          // If open the filter mode, please use this API to get the touch pad count.
          touch_pad_read_raw_data(i, &touch_value);
          touch_pad_read_filtered(i, &touch_filter_value);
          if (touch_value > 1100) {
            touch_val = 0;
          } else touch_val = 1;
          sprintf(buff,"{\"id\":%d, \"value\":%d, \"filter_value\":%d},", i, touch_val, touch_filter_value);
#else
          touch_pad_read(i, &touch_value);
          sprintf(buff,"{\"id\":%d, \"value\":%d},", i, touch_val);
#endif
          strcat(buttons_service_message,buff);
          if (touch_val)
            buttons_service_message_ready = true;
      }
      //printf("\n");
      buttons_service_message[strlen(buttons_service_message)-1] = 0; //remove trailing comma
      strcat(buttons_service_message,"]}");
      //printf("buttons_service: %s\n", buttons_service_message);
      vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
int buttons_main() {
  xTaskCreate(&buttons_service, "buttons_service_task", 5000, NULL, 5, NULL);
  return 0;
}
