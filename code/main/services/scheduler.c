char schedule_service_message[2000];
bool schedule_service_message_ready = false;

static int check_schedule()
{
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

  //printf("offset is %lu\n",tv.tv_sec - current_time);

	// if (tv.tv_sec - (*last) < secs)
	// 	return 0;
  //
	// *last = tv.tv_sec;
  //

  printf("time of day is %lu:%lu:%lu\n",hours,minutes,seconds);
	return 1;
}

static void schedule_service(void *pvParameter)
{
  uint32_t io_num;
  printf("schedule service\n");
  int previous_state = 0;

  while (1) {
    //printf("schedule service loop\n");
    check_schedule();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int schedule_main() {
  printf("starting schedule service\n");
  xTaskCreate(&schedule_service, "schedule_service_task", 5000, NULL, 5, NULL);
  return 0;
}
