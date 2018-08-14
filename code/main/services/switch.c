#include "drivers/ssr.c"

char switch_service_message[2000];
char switch_service_message_in[2000];
int current_switch_level = 0;

int
setSwitch(int level)
{
  lwsl_notice("set switch value: %d\n",level);
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, level);
  return level;
}

int
incSwitch(int amount)
{
  lwsl_notice("increment by: %d\n",amount);
  int new_switch_level = current_switch_level + amount;
  if (new_switch_level >= 100) current_switch_level=100;
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, current_switch_level);
  return current_switch_level;
}

int
decSwitch(int amount)
{
  lwsl_notice("decrement by: %d\n",amount);
  int new_switch_level = current_switch_level - amount;
  if (new_switch_level <= 0) current_switch_level=0;
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, current_switch_level);
  return current_switch_level;
}

int
fadeSwitch(int start, int stop, int duration)
{
  uint32_t angle, count;
  for (count = start; count < stop; count++) {
      angle = servo_per_degree_init(count);
      printf("Angle: %d | pulse width: %dus\n", count, angle);
      mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
      vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
  }
  return 0;
}

static void
switch_service(void *pvParameter)
{
  ssr_main();
  while (1) {

    if (switch_payload) {
      //int level = cJSON_GetObjectItem(switch_payload,"level")->valueint;

      if (cJSON_GetObjectItem(switch_payload,"level")) {
        int level = cJSON_GetObjectItem(switch_payload,"level")->valueint;
        setSwitch(level);
        lwsl_notice("[switch_service] level %d\n",level);
      }

      if (cJSON_GetObjectItem(switch_payload,"increment")) {
        int increment = cJSON_GetObjectItem(switch_payload,"increment")->valueint;
        incSwitch(increment);
        lwsl_notice("[switch_service] increment %d\n",increment);
      }

      if (cJSON_GetObjectItem(switch_payload,"decrement")) {
        int decrement = cJSON_GetObjectItem(switch_payload,"decrement")->valueint;
        incSwitch(decrement);
        lwsl_notice("[switch_service] decrement %d\n",decrement);
      }

      if (cJSON_GetObjectItem(switch_payload,"fade")) {
        int fade = cJSON_GetObjectItem(switch_payload,"fade")->valueint;
        //incSwitch(decrement);
        fadeSwitch(0,fade,0);
        lwsl_notice("[switch_service] fade %d\n",fade);
      }

      //current_switch_level = fadeSwitch(0,20,0);
      switch_payload = NULL;
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

int
switch_main()
{
  printf("starting switch service\n");
  xTaskCreate(&switch_service, "switch_service_task", 5000, NULL, 5, NULL);
  return 0;
}
