#include "drivers/pir.c"

char motion_service_message[2000];
bool motion_service_message_ready = false;

static void motion_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
  printf("motion service loop\n");
  int previous_state = 0;
  while (1) {

      int state = get_motion_state();

      if (state){

        sprintf(motion_service_message,""
        "{\"event_type\":\"motion/active\","
        " \"payload\":{\"type\":\"PIR\","
        "\"channel_0\":%d, \"channel_1\":%d, \"channel_tmp\":%d}}"
        , motion_data_0, motion_data_1, motion_tmp);

        printf("%s\n", motion_service_message);

        previous_state = state;
        motion_service_message_ready = true;

        vTaskDelay(250 / portTICK_PERIOD_MS); //debounce
        pir_debounce = false;
      } else vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

int motion_main() {
  printf("starting motion service\n");
  xTaskCreate(&motion_service, "motion_service_task", 5000, NULL, 5, NULL);
  return 0;
}
