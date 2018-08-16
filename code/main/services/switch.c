#include "drivers/ssr.c"

char switch_service_message[2000];
char switch_service_message_in[2000];
int current_switch_level = 0;
int pwm_factor = 2;

void
setPWM(int level) {
  int pwm_value = level * pwm_factor;
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, pwm_value);
}

int
setSwitch(int level)
{
  lwsl_notice("set switch value: %d\n",level);
  setPWM(level);
  mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, level);
  return level;
}

int
toggleSwitch()
{
  int new_switch_level;
  if (current_switch_level > 25) {
    new_switch_level = 0;
  } else {
    new_switch_level = 100;
  }

  lwsl_notice("toggle switch from %d to %d\n",current_switch_level,new_switch_level);
  setPWM(new_switch_level);
  current_switch_level = new_switch_level;
  return current_switch_level;
}

int
incSwitch(int amount)
{
  int new_switch_level = current_switch_level + amount;
  if (new_switch_level >= 100) new_switch_level=100;
  setPWM(new_switch_level);
  lwsl_notice("increment %d by %d\n",current_switch_level,amount);
  current_switch_level = new_switch_level;
  return current_switch_level;
}

int
decSwitch(int amount)
{
  int new_switch_level = current_switch_level - amount;
  if (new_switch_level <= 0) current_switch_level=0;
  setPWM(new_switch_level);
  lwsl_notice("decrement %d by %d\n",current_switch_level,amount);
  current_switch_level = new_switch_level;
  return current_switch_level;
}

int
fadeSwitch(int start, int stop, int duration)
{
  uint32_t angle, count;
  for (count = start; count < stop; count++) {
      angle = servo_per_degree_init(count);
      printf("Angle: %d | pulse width: %dus\n", count, angle);
      setPWM(angle);
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

      if (cJSON_GetObjectItem(switch_payload,"level")) {
        int level = cJSON_GetObjectItem(switch_payload,"level")->valueint;
        setSwitch(level);
        lwsl_notice("[switch_service] level %d\n",level);
      }

      if (cJSON_GetObjectItem(switch_payload,"toggle")) {
        toggleSwitch();
        lwsl_notice("[switch_service] toggle %d\n",current_switch_level);
      }

      if (cJSON_GetObjectItem(switch_payload,"increment")) {
        int increment = cJSON_GetObjectItem(switch_payload,"increment")->valueint;
        incSwitch(increment);
        lwsl_notice("[switch_service] increment %d\n",increment);
      }

      if (cJSON_GetObjectItem(switch_payload,"decrement")) {
        int decrement = cJSON_GetObjectItem(switch_payload,"decrement")->valueint;
        decSwitch(decrement);
        lwsl_notice("[switch_service] decrement %d\n",decrement);
      }

      if (cJSON_GetObjectItem(switch_payload,"fade")) {
        int fade = cJSON_GetObjectItem(switch_payload,"fade")->valueint;
        fadeSwitch(0,fade,0);
        lwsl_notice("[switch_service] level %d\n",fade);
      }

      if (cJSON_GetObjectItem(switch_payload,"fade")) {
        /*int fade = cJSON_GetObjectItem(switch_payload,"fade")->valueint;
        fadeSwitch(0,fade,0);
        lwsl_notice("[switch_service] fade %d\n",fade);*/
      }

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
