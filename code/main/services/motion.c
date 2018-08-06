#include "drivers/pir.c"

static void motion_service(void *pvParameter)
{
  pir_main();
  uint32_t io_num;
printf("motion service loop0\n");
  while (1) {
    printf("motion service loop\n");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    
    if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
        printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
    }
  }
  
  /*for(;;) {
      if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
          printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
      }
  }*/
}
int motion_main() {
  printf("starting motion service\n");
  xTaskCreate(&motion_service, "motion_service_task", 5000, NULL, 5, NULL);
  return 0;
}
