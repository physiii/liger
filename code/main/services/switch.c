#include "drivers/ssr.c"

char switch_service_message[2000];
char switch_service_message_in[2000];
bool switch_service_message_ready = false;
static void switch_service(void *pvParameter)
{
  //ssr_main();
  //printf("switch service loop0\n");
  while (1) {
    if (strcmp(switch_service_message_in,"")) {
          printf("new switch service message\n",switch_service_message_in);
          strcpy(switch_service_message_in,"");
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

int switch_main() {
  printf("starting switch service\n");
  xTaskCreate(&switch_service, "switch_service_task", 5000, NULL, 5, NULL);
  return 0;
}
