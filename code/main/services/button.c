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
char button_direction[100];

void createButtonServiceMessage(int state) {
  cJSON *level_json = NULL;
  int level;

  switch(state) {
    case BUTTON_CENTER:
      strcpy(button_direction,"center");
      printf("BUTTON_CENTER\n");
      break;

    case BUTTON_UP:
      strcpy(button_direction,"up");
      printf("BUTTON_UP\n");
      break;

    case BUTTON_RIGHT:
      strcpy(button_direction,"right");
      printf("BUTTON_RIGHT\n");
      break;

    case BUTTON_DOWN:
      strcpy(button_direction,"down");
      printf("BUTTON_DOWN\n");
      break;

    case BUTTON_LEFT:
      strcpy(button_direction,"left");
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
      break;
  }

  snprintf(buttons_service_message,sizeof(buttons_service_message),""
  "{\"event_type\":\"service/state\","
  " \"payload\":{\"service_id\":\"button_1\",\"state\":{\"value\":\"%s\"}}}"
  , button_direction);
  printf("%s\n", buttons_service_message);
  buttons_service_message_ready = true;
}

void createDimmerServiceMessage(int state) {
  cJSON *level_json = NULL;
  int level;

  switch(state) {
    case BUTTON_CENTER:
      dimmer_payload = cJSON_CreateObject();
      cJSON *toggle = cJSON_CreateBool(true);
      cJSON_AddItemToObject(dimmer_payload, "toggle", toggle);
      strcpy(button_direction,"center");
      printf("BUTTON_CENTER\n");
      break;

    case BUTTON_UP:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(255);
      cJSON_AddItemToObject(dimmer_payload, "level", level_json);
      strcpy(button_direction,"up");
      printf("BUTTON_UP\n");
      break;

    case BUTTON_RIGHT:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(30);
      cJSON_AddItemToObject(dimmer_payload, "increment", level_json);
      strcpy(button_direction,"right");
      printf("BUTTON_RIGHT\n");
      break;

    case BUTTON_DOWN:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(0);
      cJSON_AddItemToObject(dimmer_payload, "level", level_json);
      strcpy(button_direction,"down");
      printf("BUTTON_DOWN\n");
      break;

    case BUTTON_LEFT:
      dimmer_payload = cJSON_CreateObject();
      level_json = cJSON_CreateNumber(30);
      cJSON_AddItemToObject(dimmer_payload, "decrement", level_json);
      strcpy(button_direction,"left");
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
      // printf("Button state:\t%s\t%d\n", button_direction, state);
      if (state){

        if (dimmer_enabled) createDimmerServiceMessage(state);
        createButtonServiceMessage(state);
        // previous_state = state;

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
