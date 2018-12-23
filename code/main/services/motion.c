#include "drivers/pir.c"

char motion_service_message[2000];
bool motion_service_message_ready = false;
int motion_light_on_value = 100;
bool debounce_motion = false;
unsigned int debounce_flag = 0;

static void motion_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
  printf("motion service loop\n");
  int previous_state = 0;
  int debounce_ms = 1 * 1000;
  while (1) {

      bool state = get_motion_state();

      if (state){

        //create service message
        sprintf(motion_service_message,""
        "{\"event_type\":\"motion/active\","
        " \"payload\":{\"type\":\"PIR\","
        "\"channel_0\":%d, \"channel_1\":%d, \"channel_tmp\":%d}}"
        , motion_data_0, motion_data_1, motion_tmp);
        printf("%s\n", motion_service_message);
        previous_state = state;
        motion_service_message_ready = true;

        //turn on dimmer
        if (current_brightness < motion_light_on_value) {
          cJSON *level_json = NULL;
          dimmer_payload = cJSON_CreateObject();
          level_json = cJSON_CreateNumber(motion_light_on_value);
          cJSON_AddItemToObject(dimmer_payload, "level", level_json);
        }

      }

      vTaskDelay(250 / portTICK_PERIOD_MS);

  }
}

int motion_main() {
  printf("starting motion service\n");
  xTaskCreate(&motion_service, "motion_service_task", 5000, NULL, 5, NULL);
  return 0;
}
