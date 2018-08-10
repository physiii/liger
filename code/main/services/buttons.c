#include "drivers/touch.c"

char buttons_service_message[2000];
bool buttons_service_message_ready = false;
static void buttons_service(void *pvParameter)
{

  touch_main();
  int previous_state = 0;
  while (1) {
      int state = get_dpad_state();
      sprintf(buttons_service_message,""
      "{\"event_type\":\"button/\pressed\","
      " \"payload\":{\"type\":\"dpad\",\"value\":%d}}"
      , get_dpad_state());

      //printf("%s\n", buttons_service_message);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      if (state != previous_state){
        previous_state = state;
        buttons_service_message_ready = true;
      }
  }
}
int buttons_main() {
  xTaskCreate(&buttons_service, "buttons_service_task", 5000, NULL, 5, NULL);
  return 0;
}
