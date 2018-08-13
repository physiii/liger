#include "drivers/ssr.c"

char switch_service_message[2000];
char switch_service_message_in[2000];
int current_switch_level = 0;

int
setSwitch(int level)
{
  lwsl_notice("setting switch value: %d\n",current_switch_level);
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, level);
  return 0;
}

int
incSwitch(int amount)
{
  lwsl_notice("increment by: %d\n",amount);
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, current_switch_level+amount);
  return 0;
}

int
decSwitch(int amount)
{
  lwsl_notice("decrement by: %d\n",amount);
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, current_switch_level-amount);
  return 0;
}

int
fadeSwitch(int start, int stop, int duration)
{
  uint32_t angle, count;
  for (count = 0; count < stop; count++) {
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
      lwsl_notice("incoming payload for switch service\n");
      int level = cJSON_GetObjectItem(switch_payload,"level")->valueint;
      printf("level: %d\n",level);
      //int increment = cJSON_GetObjectItem(switch_payload,"increment")->valueint;
      current_switch_level = setSwitch(level);
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
