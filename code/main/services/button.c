#include "drivers/touch.c"

#define BUTTON_RELEASE 0
#define BUTTON_CENTER 1
#define BUTTON_UP 6
#define BUTTON_RIGHT 7
#define BUTTON_DOWN 8
#define BUTTON_LEFT 9
#define BUTTON_UP_RIGHT 2
#define BUTTON_RIGHT_DOWN 3
#define BUTTON_DOWN_LEFT 4
#define BUTTON_LEFT_UP 5

char buttons_service_message[2000];
bool buttons_service_message_ready = false;

void button_event_handler(int state) {
  cJSON *level_json = NULL;
  int level;

  switch(state) {
    case BUTTON_CENTER:
      dimmer_payload = cJSON_CreateObject();
      cJSON *toggle = cJSON_CreateBool(true);
      cJSON_AddItemToObject(dimmer_payload, "toggle", toggle);
      printf("BUTTON_CENTER\n");
      break;

    case BUTTON_UP:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(255);
      cJSON_AddItemToObject(dimmer_payload, "level", level_json);
      printf("BUTTON_UP\n");
      break;

    case BUTTON_RIGHT:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(30);
      cJSON_AddItemToObject(dimmer_payload, "increment", level_json);
      printf("BUTTON_RIGHT\n");
      break;

    case BUTTON_DOWN:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(0);
      cJSON_AddItemToObject(dimmer_payload, "level", level_json);
      printf("BUTTON_DOWN\n");
      break;

    case BUTTON_LEFT:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(30);
      cJSON_AddItemToObject(dimmer_payload, "decrement", level_json);
      printf("BUTTON_LEFT\n");
      break;

    case BUTTON_UP_RIGHT:
      break;

    case BUTTON_RIGHT_DOWN:
      break;

    case BUTTON_DOWN_LEFT:
      break;

    case BUTTON_LEFT_UP:
      break;

    case BUTTON_RELEASE:
      break;

    default:
      //dimmer_payload = NULL;
      //lwsl_err("bad dpad state");
      break;
  }
}

static void
button_service(void *pvParameter)
{

  touch_main();
  int previous_state = 0;
  while (1) {

      int state = get_dpad_state();

      if (state){

        button_event_handler(state);

        snprintf(buttons_service_message,sizeof(buttons_service_message),""
        "{\"event_type\":\"button/\pressed\","
        " \"payload\":{\"type\":\"dpad\",\"value\":%d}}"
        , state);

        printf("%s\n", buttons_service_message);

        previous_state = state;
        buttons_service_message_ready = true;

        vTaskDelay(250 / portTICK_PERIOD_MS); //debounce
        tp_debounce = false;
      } else vTaskDelay(250 / portTICK_PERIOD_MS);
  }

}

int
buttons_main() {
  xTaskCreate(&button_service, "button_service_task", 5000, NULL, 5, NULL);
  return 0;
}
