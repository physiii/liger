char schedule_service_message[2000];
bool schedule_service_message_ready = false;

cJSON *schedule_ids = NULL;

int check_schedule() {
	struct timeval tv;
  int seconds_in_minute = 60;
  int minutes_in_hour = 60;
  int hours_in_day = 24;
  int central_time_hours = -6;
  long int timezone_offset = central_time_hours*minutes_in_hour*seconds_in_minute;
	gettimeofday(&tv, NULL);

  long int seconds_into_day = tv.tv_sec % 86400 + timezone_offset;
  long int minutes_into_day = seconds_into_day / seconds_in_minute;
  long int hours_into_day = minutes_into_day / minutes_in_hour;

  long int hours = hours_into_day;
  long int minutes = minutes_into_day % minutes_in_hour;
  long int seconds = seconds_into_day % seconds_in_minute;

  char id_str[20];
  char events_arr[100][20];
  int index = 0;

  cJSON *id =  NULL;
  cJSON_ArrayForEach(id, schedule_ids)
  {
    strcpy(id_str,id->valuestring);
    printf("looking at schedule id ...%s\n", id_str);

    char * events_str = get_char(id_str);
    cJSON *events_obj = cJSON_Parse(events_str);
    free(events_str);
    cJSON *events = cJSON_GetObjectItemCaseSensitive(events_obj,"event_list");

    cJSON *event = NULL;
    cJSON_ArrayForEach(event, events)
    {
      cJSON *id = cJSON_GetObjectItemCaseSensitive(event,"event_id");
      char current_id[100];
      sprintf(current_id,"%s",id->valuestring);
      int _seconds_into_day = cJSON_GetObjectItem(event,"seconds_into_day")->valueint;
      int difference = _seconds_into_day - seconds_into_day;
      printf("[%s] currently %lu, going off at %d\n",current_id,seconds_into_day,_seconds_into_day);
      //printf("difference: %d\n",difference);
      if (difference == 0) {
        cJSON *state = cJSON_GetObjectItem(event,"state");
        int level = cJSON_GetObjectItem(state,"level")->valueint;
        set_brightness(level);
        printf("[%s] setting level to %d\n",current_id,level);
      }
    }

    cJSON_Delete(events_obj);
  }

  //printf("time of day is %lu:%lu:%lu\n",hours,minutes,seconds);
  printf("seconds into day %lu\n",seconds_into_day);
	return 1;
}

int add_service_id(char * id, cJSON * schedule_ids_json) {
  cJSON *service_ids = cJSON_CreateArray();
  cJSON *new_service_id =  cJSON_CreateString(id);

  service_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids_json,"service_ids");
  cJSON_AddItemToArray(service_ids, new_service_id);
  cJSON *new_schedule_ids = cJSON_CreateObject();
  cJSON_AddItemToObject(new_schedule_ids, "service_ids", service_ids);

  char schedule_str[2000];
  strcpy(schedule_str,cJSON_PrintUnformatted(new_schedule_ids));
  printf("add_service_id %s\n",schedule_str);
  store_char("schedule_ids", schedule_str);
  return 0;
}

int create_schedule_ids() {
  cJSON *schedule = cJSON_CreateObject();
  cJSON *id_arr = cJSON_CreateArray();
  cJSON_AddItemToObject(schedule, "service_ids", id_arr);
  char *schedule_str = cJSON_PrintUnformatted(schedule);
  store_char("schedule_ids",schedule_str);
  printf("create_schedule_ids\n");
  return 0;
}

int check_service_id(char * service_id) {
  char * schedule_ids = get_char("schedule_ids");
  if (strcmp(schedule_ids,"")==0) {
    create_schedule_ids();
  }
  //printf("schedule_ids...\n%s\n",schedule_ids);

  cJSON *schedule_ids_json = cJSON_Parse(schedule_ids);;
  bool id_found = false;

  cJSON *service_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids_json,"service_ids");
  cJSON *service_id_json =  NULL;
  cJSON_ArrayForEach(service_id_json, service_ids)
  {
    char current_id[15];
    sprintf(current_id,"%s",service_id_json->valuestring);
    //printf("service id: %s\n",current_id);
    if (strcmp(current_id,service_id)==0) {
      id_found = true;
    }
  }

  if (!id_found) {
    add_service_id(service_id,schedule_ids_json);
  } else {
    printf("service id (%s) found in schedule ids.\n",service_id);
  }

  return 0;
}

int store_event() {
  cJSON *event_list = NULL;
  char service_id[250];
  sprintf(service_id,"%s",cJSON_GetObjectItem(schedule_payload,"service_id")->valuestring);
  check_service_id(service_id);

  char event_id[250];
  sprintf(event_id,"%s",cJSON_GetObjectItem(schedule_payload,"event_id")->valuestring);

  char * events = get_char(service_id);
  cJSON *events_obj = cJSON_Parse(events);

  if (cJSON_GetObjectItemCaseSensitive(events_obj,"event_list")) {
    //printf("checking event_list for event_id %s\n",event_id);

    cJSON *event_ids = cJSON_GetObjectItemCaseSensitive(events_obj,"event_list");
    cJSON *event_id_obj =  NULL;
    int index = 0;
    int index_found = 0;
    bool found_index = false;
    cJSON_ArrayForEach(event_id_obj, event_ids)
    {
      char current_id[100];
      cJSON *id = cJSON_GetObjectItemCaseSensitive(event_id_obj,"event_id");
      sprintf(current_id,"%s",id->valuestring);
      //printf("current event id: %s, looking for %s\n",current_id,event_id);
      if (strcmp(current_id,event_id)==0) {
         //printf("deleting event id at index %d\n",index);
         index_found = index;
         found_index = true;
      }
      index++;
    }

    if (index_found >= 0) {
      cJSON_DeleteItemFromArray(event_ids,index_found);
    }

    event_list = event_ids;
  }
  cJSON_AddItemToArray(event_list, schedule_payload);
  cJSON *new_event_list = cJSON_CreateObject();
  cJSON_AddItemToObject(new_event_list, "event_list", event_list);
  char new_event_list_str[1000];
  strcpy(new_event_list_str,cJSON_PrintUnformatted(new_event_list));
  printf("storing new_event_list for %s...\n%s\n",service_id,new_event_list_str);
  store_char(service_id,new_event_list_str);
  return 0;
}

int handle_action(char * action) {
  printf("handle_action %s\n",action);
	if (strcmp(action,"add")==0) {
    store_event();
	}
	return 0;
}

int load_schedule_from_flash() {

  char * schedule_ids_str = get_char("schedule_ids");
  cJSON *schedule_ids_obj = cJSON_Parse(schedule_ids_str);

  if (cJSON_GetObjectItemCaseSensitive(schedule_ids_obj,"service_ids")) {

    schedule_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids_obj,"service_ids");
    cJSON *service_id =  NULL;
    int index = 0;
    printf("loaded schedule ids...\n%s\n",schedule_ids_str);

  } else {
    lwsl_err("no schedule found\n");
  }
  return 0;
}

static void schedule_service(void *pvParameter) {
  uint32_t io_num;
  printf("schedule service\n");
  int previous_state = 0;
  load_schedule_from_flash();
  int cnt = 0;
  while (1) {

      //incoming messages from other services
      if (schedule_payload) {

        if (cJSON_GetObjectItem(schedule_payload,"action")) {
          char action[50];
          sprintf(action,"%s",cJSON_GetObjectItem(schedule_payload,"action")->valuestring);
          lwsl_notice("[schedule_service] schedule action...%s\n",action);
          handle_action(action);
        }

        schedule_payload = NULL;
      }

      check_schedule();
      printf("cnt: %d\n",cnt++);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int schedule_main() {
  printf("starting schedule service\n");
  xTaskCreate(&schedule_service, "schedule_service_task", 5000, NULL, 5, NULL);
  return 0;
}
