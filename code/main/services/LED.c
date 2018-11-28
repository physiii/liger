#include "drivers/ws2812b.c"

char LED_service_message[2000];
char LED_service_message_in[2000];
int current_LED_level = 0;
int pwm_factor = 2;

int setLED(int r, int g, int b) {
  lwsl_notice("set LED [%d,%d,%d]\n",r,g,b);
  set_pixel_by_index(0, r, g, b, 1);
  return 0;
}

int toggleLED() {
  int new_LED_level;
  if (current_LED_level > 25) {
    new_LED_level = 0;
  } else {
    new_LED_level = 100;
  }

  lwsl_notice("toggle LED from %d to %d\n",current_LED_level,new_LED_level);
  setLED(new_LED_level,new_LED_level,new_LED_level);
  current_LED_level = new_LED_level;
  return current_LED_level;
}

int fadeLED(int start, int stop, int duration) {
  uint32_t angle, count;
  /*for (count = start; count < stop; count++) {
      angle = servo_per_degree_init(count);
      printf("Angle: %d | pulse width: %dus\n", count, angle);
      setLED(angle);
      vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
  }*/
  return 0;
}

static void LED_service(void *pvParameter) {

  while (1) {

    //incoming messages from other services
    if (LED_payload) {

      if (cJSON_GetObjectItem(LED_payload,"level")) {
        int level = cJSON_GetObjectItem(LED_payload,"level")->valueint;
        setLED(level,level,level);
        lwsl_notice("[LED_service] level %d\n",level);
      }

      if (cJSON_GetObjectItem(LED_payload,"toggle")) {
        toggleLED();
        lwsl_notice("[LED_service] toggle %d\n",current_LED_level);
      }

      if (cJSON_GetObjectItem(LED_payload,"increment")) {
        int increment = cJSON_GetObjectItem(LED_payload,"increment")->valueint;
        //incLED(increment);
        lwsl_notice("[LED_service] increment %d\n",increment);
      }

      if (cJSON_GetObjectItem(LED_payload,"decrement")) {
        int decrement = cJSON_GetObjectItem(LED_payload,"decrement")->valueint;
        //decLED(decrement);
        lwsl_notice("[LED_service] decrement %d\n",decrement);
      }

      if (cJSON_GetObjectItem(LED_payload,"fade")) {
        int fade = cJSON_GetObjectItem(LED_payload,"fade")->valueint;
        fadeLED(0,fade,0);
        lwsl_notice("[LED_service] level %d\n",fade);
      }

      if (cJSON_GetObjectItem(LED_payload,"fade")) {
        /*int fade = cJSON_GetObjectItem(LED_payload,"fade")->valueint;
        fadeLED(0,fade,0);
        lwsl_notice("[LED_service] fade %d\n",fade);*/
      }

      LED_payload = NULL;
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

int LED_main() {
  ws2812b_main();
  printf("starting LED service\n");
  xTaskCreate(&LED_service, "LED_service_task", 5000, NULL, 5, NULL);
  return 0;
}
