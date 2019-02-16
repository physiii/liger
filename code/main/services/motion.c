#include "drivers/PYQ2898.c"

char motion_service_message[2000];
bool motion_service_message_ready = false;
bool debounce_motion = false;
unsigned int debounce_flag = 0;

void createMotionServiceMessage () {
  sprintf(motion_service_message,""
  "{\"event_type\":\"motion/active\","
  " \"payload\":{\"type\":\"PIR\","
  "\"level\":%d, \"duration\":%d, \"channel_tmp\":%d}}"
  , motion_level, motion_duration, motion_tmp);
  printf("%s\n", motion_service_message);

  motion_service_message_ready = true;
}

void createDimmerServiceMessage () {
  //turn on dimmer
  cJSON *json = NULL;
  dimmer_payload = cJSON_CreateObject();
  json = cJSON_CreateNumber(1);
  cJSON_AddItemToObject(dimmer_payload, "on", json);
}


static void motion_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
  printf("motion service loop\n");

  while (1) {

      int state = get_motion_state();
      if (state){

        createMotionServiceMessage();

        createDimmerServiceMessage();

        if (isArmed()) {
          createAlarmServiceMessage();
        }

        debounce_pir(10);
      }

      vTaskDelay(250 / portTICK_PERIOD_MS);

  }
}

int motion_main() {
  printf("starting motion service\n");
  xTaskCreate(&motion_service, "motion_service_task", 5000, NULL, 5, NULL);
  return 0;
}
