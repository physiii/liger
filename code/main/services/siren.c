#include "drivers/siren.c"

char switch_service_message[2000];
char switch_service_message_in[2000];

static void
siren_service(void *pvParameter)
{

  while (1) {

    //incoming messages from other services
    if (siren_payload) {
      if (cJSON_GetObjectItem(siren_payload,"level")) {
        int level = cJSON_GetObjectItem(siren_payload,"level")->valueint;
        setSiren(level);
        lwsl_notice("[siren_service] level %d\n",level);
      }

      if (cJSON_GetObjectItem(siren_payload,"chirp")) {
        int chirp = cJSON_GetObjectItem(siren_payload,"chirp")->valueint;;
        chirpSiren(chirp);
        lwsl_notice("[siren_service] chirp %d\n",chirp);
      }

      siren_payload = NULL;
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

int
siren_main()
{
  siren_driver_main();
  printf("starting siren service\n");
  xTaskCreate(&siren_service, "siren_service_task", 5000, NULL, 5, NULL);
  return 0;
}
