#include "drivers/PYQ2898.c"

char motion_service_message[2000];
bool motion_service_message_ready = false;
bool debounce_motion = false;
unsigned int debounce_flag = 0;
int motion_duration = 0;
int motion_duration_threshold = 2;

void createMotionServiceMessage () {
  sprintf(motion_service_message,""
  "{\"event_type\":\"motion/active\","
  " \"payload\":{\"type\":\"PIR\","
  "\"level\":%d, \"duration\":%d}}"
  , motion_level, motion_duration);
  printf("%s\n", motion_service_message);

  motion_service_message_ready = true;
}

static void motion_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
  printf("motion service loop\n");

  while (1) {
    if (get_motion_state()){
      if (motion_level > motion_threshold) motion_duration++;
      if (motion_duration > motion_duration_threshold) {
        createMotionServiceMessage();
        // createDimmerServiceMessage(BUTTON_UP);
        if (isArmed()) createAlarmServiceMessage();
      }
    } else {
      motion_duration = 0;
    }

    //incoming messages from other services
    if (motion_payload) {
      if (cJSON_GetObjectItem(motion_payload,"motion_threshold")) {
        int mode = cJSON_GetObjectItem(motion_payload,"motion_threshold")->valueint;
        // armSystem(mode);
        // store_alarm_state(mode);
        lwsl_notice("[motion_service] mode %d\n",mode);
      }

      motion_payload = NULL;
    }

    // printf("Motion Level:\t%d\n",motion_level);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}

int motion_main() {
  printf("starting motion service\n");
  xTaskCreate(&motion_service, "motion_service_task", 5000, NULL, 5, NULL);
  return 0;
}
