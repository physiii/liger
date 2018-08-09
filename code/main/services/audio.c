#include "drivers/microphone.c"

static void audio_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
  //printf("audio service loop0\n");
  microphone_main();
  while (1) {
    //printf("audio service loop\n");
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

int audio_main() {
  printf("starting audio service\n");
  xTaskCreate(&audio_service, "audio_service_task", 5000, NULL, 5, NULL);
  return 0;
}
