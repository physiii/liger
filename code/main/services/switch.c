#include "drivers/ssr.c"

static void switch_service(void *pvParameter)
{
  //ssr_main();
  uint32_t io_num;
  //printf("switch service loop0\n");
  while (1) {
    vTaskDelay(200 / portTICK_PERIOD_MS);
    //printf("switch service loop\n");
  }
}

int switch_main() {
  printf("starting switch service\n");
  xTaskCreate(&switch_service, "switch_service_task", 5000, NULL, 5, NULL);
  return 0;
}
