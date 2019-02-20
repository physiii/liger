#include "drivers/SPH0645LM4H.c"

int mic_threshold = 1000;
char audio_service_message[2000];
bool audio_service_message_ready = false;
bool activate_threshold = false;

static void audio_service(void *pvParameter)
{
  uint32_t io_num;
  SPH0645LM4H_main();

  while (1) {
    int level = get_audio_level();

    // if (level < mic_threshold && !activate_threshold) {
    //   printf("mic activated\n");
    //   activate_threshold = true;
    // }

    if (level > mic_threshold) {
      //create service message
      sprintf(audio_service_message,""
      "{\"event_type\":\"mic/active\","
      " \"payload\":{\"type\":\"sound\","
      "\"level\":%d}}"
      , level);
      printf("%s\n", audio_service_message);
      audio_service_message_ready = true;

      if (isArmed()) createAlarmServiceMessage();

      //turn on dimmer
      cJSON *json = NULL;
      dimmer_payload = cJSON_CreateObject();
      json = cJSON_CreateNumber(1);
      cJSON_AddItemToObject(dimmer_payload, "on", json);
    }

    //incoming messages from other services
    if (audio_payload) {
      if (cJSON_GetObjectItem(audio_payload,"mode")) {
        int mode = cJSON_GetObjectItem(audio_payload,"mode")->valueint;
        armSystem(mode);
        store_alarm_state(mode);
        lwsl_notice("[audio_service] mode %d\n",mode);
      }

      audio_payload = NULL;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int audio_main() {
  printf("starting audio service\n");
  xTaskCreate(&audio_service, "audio_service_task", 5000, NULL, 5, NULL);
  return 0;
}
