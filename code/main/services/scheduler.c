char schedule_service_message[2000];
bool schedule_service_message_ready = false;

cJSON *schedule_ids = NULL;
cJSON *global_event_list[100];
int global_event_list_size = 0;
int cnt = 0;

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

  char id_str[10];
  char events_arr[100][20];
  int index = 0;


  cJSON *id =  NULL;
	//cJSON_ArrayForEach(id, schedule_ids)
  for (int i = 0; i < global_event_list_size; i++)
  {
		//printf("looping through global_event_list\n");
		// char * event_list = cJSON_PrintUnformatted(global_event_list[i]);
    // strcpy(id_str,event_list);
    // printf("(%d) looking at event_list: %s\n",cnt++, event_list);

    // char * events_str = get_char(id_str);
    // cJSON *events_obj = cJSON_Parse(events_str);
    // free(events_str);

    cJSON *events = cJSON_GetObjectItemCaseSensitive(global_event_list[i],"event_list");
    cJSON *event = NULL;
    cJSON_ArrayForEach(event, events)
    {
			if (cJSON_GetObjectItemCaseSensitive(event,"event_id")) {
      	cJSON *id = cJSON_GetObjectItemCaseSensitive(event,"event_id");
      	char current_id[100];
      	sprintf(current_id,"%s",id->valuestring);
				int _seconds_into_day = cJSON_GetObjectItem(event,"seconds_into_day")->valueint;
	      int difference = _seconds_into_day - seconds_into_day;
	      printf("(%d)[%s] currently %lu, going off at %d\n",cnt++,current_id,seconds_into_day,_seconds_into_day);
	      if (difference == 0) {
	        cJSON *state = cJSON_GetObjectItem(event,"state");
	        int level = cJSON_GetObjectItem(state,"level")->valueint;
	        set_brightness(level);
	        printf("[%s] setting level to %d\n",current_id,level);
	      }
			}
    }
    // cJSON_Delete(events_obj);
  }

  //printf("time of day is %lu:%lu:%lu\n",hours,minutes,seconds);
  //printf("seconds into day %lu\n",seconds_into_day);
	return 1;
}

int add_service_id(char * id) {
  cJSON *service_ids = cJSON_CreateArray();
  cJSON *new_service_id =  cJSON_CreateString(id);

  //service_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids,"service_ids");
  cJSON_AddItemToArray(schedule_ids, new_service_id);
  //cJSON *new_schedule_ids = cJSON_CreateObject();
  //cJSON_AddItemToObject(new_schedule_ids, "service_ids", service_ids);

	printf("new scheduleids: %s\n",cJSON_PrintUnformatted(schedule_ids));
	//schedule_ids = new_schedule_ids;
  char schedule_str[1000];
	cJSON * schedule_obj = cJSON_CreateObject();
	cJSON_AddItemToObject(schedule_obj,"service_ids",schedule_ids);
  strcpy(schedule_str,cJSON_PrintUnformatted(schedule_obj));
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
  if (schedule_ids == NULL) {
    create_schedule_ids();
  }
  //cJSON *schedule_ids_json = cJSON_Parse(schedule_ids);;
  bool id_found = false;

  //cJSON *service_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids_json,"service_ids");
  cJSON *service_id_json =  NULL;
  cJSON_ArrayForEach(service_id_json, schedule_ids)
  {
    char current_id[15];
    sprintf(current_id,"%s",service_id_json->valuestring);
    //printf("service id: %s\n",current_id);
    if (strcmp(current_id,service_id)==0) {
      id_found = true;
    }
  }

  if (!id_found) {
    add_service_id(service_id);
  } else {
    printf("service id (%s) found in schedule ids.\n",service_id);
  }

  return 0;
}

int store_event() {

  char service_id[250];
  sprintf(service_id,"%s",cJSON_GetObjectItem(schedule_payload,"service_id")->valuestring);
  check_service_id(service_id);

  char event_id[250];
  sprintf(event_id,"%s",cJSON_GetObjectItem(schedule_payload,"event_id")->valuestring);

	for (int i = 0; i < global_event_list_size; i++)
  {
		printf("looping global_event_list_size\n");
    cJSON *events = cJSON_GetObjectItemCaseSensitive(global_event_list[i],"event_list");
    cJSON *event = NULL;
		int index = 0;
    cJSON_ArrayForEach(event, events)
    {
			if (cJSON_GetObjectItemCaseSensitive(event,"service_id")) {
      	cJSON *service_id = cJSON_GetObjectItemCaseSensitive(event,"service_id");
				cJSON *event_id = cJSON_GetObjectItemCaseSensitive(event,"event_id");
      	char current_service_id[50];
				char current_event_id[50];
      	sprintf(current_service_id,"%s",service_id->valuestring);
				sprintf(current_event_id,"%s",event_id->valuestring);

				if (strcmp(current_service_id,service_id)==0) {
					if (strcmp(current_event_id,event_id->valuestring)==0) {
							printf("deleting event to replace...%s %s",current_service_id,current_service_id);
							cJSON_DeleteItemFromArray(events,index);
					}
					cJSON_AddItemToArray(events,schedule_payload);

					char new_events_str[1000];
					cJSON *new_events = cJSON_CreateObject();
					cJSON_AddItemToObject(new_events, "event_list", events);
					strcpy(new_events_str,cJSON_PrintUnformatted(new_events));
					printf("storing new_event_list for %s...\n%s\n",current_service_id,new_events_str);
					store_char(current_service_id,new_events_str);
					return 0;
				}
			}
			index++;
    }
  }

  return 0;
}

int remove_event(char * service_id, char * event_id) {
	printf("removing event (%s) from service id (%s)\n",event_id,service_id);

	for (int i = 0; i < global_event_list_size; i++)
  {
    cJSON *events = cJSON_GetObjectItemCaseSensitive(global_event_list[i],"event_list");
    cJSON *event = NULL;
		int index = 0;
    cJSON_ArrayForEach(event, events)
    {
			if (cJSON_GetObjectItemCaseSensitive(event,"service_id")) {
      	cJSON *service_id = cJSON_GetObjectItemCaseSensitive(event,"service_id");
				cJSON *event_id = cJSON_GetObjectItemCaseSensitive(event,"event_id");
      	char current_service_id[50];
				char current_event_id[50];
      	sprintf(current_service_id,"%s",service_id->valuestring);
				sprintf(current_event_id,"%s",event_id->valuestring);
				if (strcmp(current_service_id,service_id)==0 && strcmp(current_event_id,event_id)==0) {
						printf("deleting event...%s %s",current_service_id,current_service_id);
						cJSON_DeleteItemFromArray(events,index);
				}
			}
			index++;
    }
  }
  return 0;
}

int handle_action(char * action) {
  printf("handle_action %s\n",action);
	if (strcmp(action,"add")==0) {
    store_event();
	}

	if (strcmp(action,"remove")==0) {
		char service_id[250];
		char event_id[250];
	  sprintf(service_id,"%s",cJSON_GetObjectItem(schedule_payload,"service_id")->valuestring);
	  sprintf(event_id,"%s",cJSON_GetObjectItem(schedule_payload,"event_id")->valuestring);
		remove_event(service_id,event_id);
	}
	return 0;
}

int load_schedule_from_flash() {

  char * schedule_ids_str = get_char("schedule_ids");
  cJSON *schedule_ids_obj = cJSON_Parse(schedule_ids_str);

	printf("schedule ids: %s\n",schedule_ids_str);
  if (cJSON_GetObjectItemCaseSensitive(schedule_ids_obj,"service_ids")) {
    schedule_ids = cJSON_GetObjectItemCaseSensitive(schedule_ids_obj,"service_ids");
    cJSON *service_id =  NULL;
    int index = 0;
    lwsl_notice("loaded schedule ids...\n%s\n",schedule_ids_str);
  } else {
    lwsl_err("no schedule found\n");
  }

	int schedule_ids_size = cJSON_GetArraySize(schedule_ids);
	//char events[schedule_ids_size][200];
	int index = 0;
	cJSON *id =  NULL;
	cJSON_ArrayForEach(id, schedule_ids)
	{
		char id_str[15];
		sprintf(id_str,"%s",id->valuestring);
		char * event_list =	get_char(id_str);
		printf("adding events\n%s\n",event_list);
		cJSON * event_list_obj = cJSON_Parse(event_list);
		global_event_list[index] = event_list_obj;
		printf("added\n");
		index++;
	}
	global_event_list_size = index;
  return 0;
}

static void schedule_service(void *pvParameter) {
  uint32_t io_num;
  printf("schedule service\n");
  int previous_state = 0;
  load_schedule_from_flash();
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
      //printf("cnt: %d\n",cnt++);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int schedule_main() {
  printf("starting schedule service\n");
	load_schedule_from_flash();
  xTaskCreate(&schedule_service, "schedule_service_task", 10000, NULL, 5, NULL);
  return 0;
}
