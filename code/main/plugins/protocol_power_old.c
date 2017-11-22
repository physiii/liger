/*
 * ws protocol handler plugin for "dumb increment"
 *
 * Copyright (C) 2010-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * These test plugins are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#if !defined (LWS_PLUGIN_STATIC)
#define LWS_DLL
#define LWS_INTERNAL
#include "../lib/libwebsockets.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif


#define VOLTAGE_CHANNEL (4)
#define CURRENT_CHANNEL (35)
#define SAMPLE_SIZE (128)
#define SAMPLE_RATE (44100)

int value[SAMPLE_SIZE];
int power_value;
char temp_str[50];
int sum = 0;
int main_voltage = 0;
int main_current = 0;
char power_req_str[1024] = "";
bool power_linked = false;
bool power_req_sent= false;

struct per_vhost_data__power {
	uv_timer_t timeout_watcher;

	TimerHandle_t timer, reboot_timer;
	struct per_session_data__esplws_scan *live_pss_list;
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;

	struct lws *cwsi;
	char json[1024];
	int json_len;
	bool is_connecting;
	int value[1024];
};

struct per_session_data__power {
	int number;
	int value;
};

//extern int value[1024];

static void
uv_timeout_cb_power(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__power *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__power, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}



void power_task(struct per_vhost_data__power *vhd)
{
	char tag[50] = "[power-protocol]";
	/*adc_timer = xTimerCreate("x", pdMS_TO_TICKS(1 / SAMPLE_RATE), 1, NULL,
		(TimerCallbackFunction_t)adc_timer_cb);
	xTimerStart(adc_timer, 0);*/
	while(1){
		//sum = adc1_get_voltage(POWER_CHANNEL);
		main_voltage = adc1_get_voltage(VOLTAGE_CHANNEL);
		//main_current = adc1_get_voltage(CURRENT_CHANNEL);
		sum = 0;
		for (int i = 0; i < 1000; i++) {
			//main_voltage+=adc1_get_voltage(VOLTAGE_CHANNEL);
			// Read ADC and obtain result in mV
			uint32_t voltage = adc1_to_voltage(ADC1_CHANNEL_6, &characteristics);
			printf("%d mV\n",voltage);
			vTaskDelay(10/portTICK_PERIOD_MS);
		}
		main_voltage = main_voltage / 1000;
		printf("%s main voltage: %d\n",tag, main_voltage);
	}
}

static int
callback_power(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[power-protocol]";

	struct per_session_data__power *pss =
			(struct per_session_data__power *)user;
	struct per_vhost_data__power *vhd =
			(struct per_vhost_data__power *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 20];
	unsigned char *p = &buf[LWS_PRE];
	char power_value_str[1024];
	int n, m;

	// initialize ADC
	adc1_config_width(ADC_WIDTH_12Bit);
	adc1_config_channel_atten(VOLTAGE_CHANNEL,ADC_ATTEN_6db);
	adc1_config_channel_atten(CURRENT_CHANNEL,ADC_ATTEN_6db);
	// Calculate ADC characteristics i.e. gain and offset factors
        esp_adc_cal_characteristics_t characteristics;
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_6db, ADC_WIDTH_12Bit,&characteristics);
	xTaskCreate(power_task, "power_task", 1024*3, &vhd, 10, NULL);

	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__power));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_power, DUMB_PERIOD, DUMB_PERIOD);
		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		if (!vhd)
			break;
	//	lwsl_notice("di: LWS_CALLBACK_PROTOCOL_DESTROY: v=%p, ctx=%p\n", vhd, vhd->context);
		uv_timer_stop(&vhd->timeout_watcher);
		uv_close((uv_handle_t *)&vhd->timeout_watcher, NULL);
		break;

	case LWS_CALLBACK_ESTABLISHED:
		pss->number = 0;
		break;

	case LWS_CALLBACK_CLOSED:
		power_linked = false;
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] sum: %d\n",sum);
		if (!token_received) break;
		//if (power_req_sent) break;
		//power_req_sent = true;
		if (!power_linked) {
        	        /*strcpy(power_req_str, "{\"mac\":\"");
			strcat(power_req_str,mac_str);
        	        strcat(power_req_str, "\",\"device_type\":[\"regulator\"]");
        	        strcat(power_req_str, ",\"cmd\":\"link\"");
        	        strcat(power_req_str, ",\"token\":\"");
        	        strcat(power_req_str, token);
        	        strcat(power_req_str, "\"");
			strcat(power_req_str,"}");
			n = lws_snprintf((char *)p, sizeof(power_req_str) - LWS_PRE, "%s", power_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,power_req_str);
			}*/
			break;
		}

		/*strcpy(power_req_str, "{\"mac\":\"");
		strcat(power_req_str,mac_str);
		strcat(power_req_str, "\",\"device_type\":[\"regulator\"]");
		strcat(power_req_str, ",\"cmd\":\"main_voltage\"");
		strcat(power_req_str, ",\"token\":\"");
		strcat(power_req_str, token);
		strcat(power_req_str, "\"");
		strcat(power_req_str, ",\"main_voltage\":\"");
		char main_voltage_str[10] = "";
		sprintf(main_voltage_str,"%d",main_voltage);
		strcat(power_req_str, main_voltage_str);
		strcat(power_req_str, "\"");
		strcat(power_req_str,"}");
		n = lws_snprintf((char *)p, sizeof(power_req_str) - LWS_PRE, "%s", power_req_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n)
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			printf("%s %s\n",tag,power_req_str);
		}*/
		vTaskDelay(100/portTICK_PERIOD_MS);
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;
		//strcpy(buttons_rx_data, (const char *)in);
		sprintf(button_command,"%s",(const char *)in);
		//printf("%s %s\n", tag, button_command);
		if (strcmp(button_command,"link")) {
			//printf("%s LINKED!!\n", tag);
			buttons_linked = true;
		}

		if (!strcmp(button_command,"light_on")) {
			printf("%s turining power_en on!\n", tag);
                        //printf("%s setting power_en pin to %d to %d\n", tag, POWER_EN, power_en_value);
			power_en_value = 100;
                        gpio_set_level(POWER_EN, power_en_value);
			//power_en(POWER_EN,power_en_value);
		}

		if (!strcmp(button_command,"light_off")) {
			printf("%s turining power_en off!\n", tag);
			power_en_value = 0;
                        //printf("%s setting power_en pin to %d to %d\n", tag, POWER_EN, power_en_value);
                        gpio_set_level(POWER_EN, power_en_value);
			//power_en(POWER_EN,power_en_value);
		}
		//button_req_sent = false;
		break;

	default:
	   	printf("callback_power: %d\n",reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_POWER \
	{ \
		"power-protocol", \
		callback_power, \
		sizeof(struct per_session_data__power), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_POWER
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_power(struct lws_context *context,
			     struct lws_plugin_capability *c)
{
	if (c->api_magic != LWS_PLUGIN_API_MAGIC) {
		lwsl_err("Plugin API %d, library API %d", LWS_PLUGIN_API_MAGIC,
			 c->api_magic);
		return 1;
	}

	c->protocols = protocols;
	c->count_protocols = ARRAY_SIZE(protocols);
	c->extensions = NULL;
	c->count_extensions = 0;

	return 0;
}

LWS_EXTERN LWS_VISIBLE int
destroy_protocol_power(struct lws_context *context)
{
	return 0;
}

#endif
