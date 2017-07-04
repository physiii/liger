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
#include "esp_system.h"
#include "nvs_flash.h"

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define BUTTON1 (4)
#define BUTTON2 (5)
#define BUTTON3 (6)
#define BUTTON4 (7)

uint8_t mac[6];
char mac_str[20];
int value[SAMPLE_SIZE];
int button_value;
char button1_str[50];
char button2_str[50];
char button3_str[50];
char button4_str[50];
int button1_sum = 0;
int button2_sum = 0;
int button3_sum = 0;
int button4_sum = 0;

struct per_vhost_data__buttons {
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

struct per_session_data__buttons {
	int number;
	int value;
};

//extern int value[1024];

static void
uv_timeout_cb_buttons(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__buttons *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__buttons, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

void adc2task(struct per_vhost_data__buttons *vhd)
{
	/*adc_timer = xTimerCreate("x", pdMS_TO_TICKS(1 / SAMPLE_RATE), 1, NULL,
		(TimerCallbackFunction_t)adc_timer_cb);
	xTimerStart(adc_timer, 0);*/
	while(1){
		//button_sum = adc1_get_voltage(BUTTON1);
		button1_sum = 0;
		button2_sum = 0;
		button3_sum = 0;
		button4_sum = 0;
		for (int i = 0; i < SAMPLE_SIZE; i++) {
			button1_sum+=adc1_get_voltage(BUTTON1);
			button2_sum+=adc1_get_voltage(BUTTON2);
			button3_sum+=adc1_get_voltage(BUTTON3);
			button4_sum+=adc1_get_voltage(BUTTON4);
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
		//printf("button3_sum: %d\n",button3_sum);
	}
}

/*uint8_t temprature_sens_read(); 
uint32_t hall_sens_read();
void read_sens_task(void *pvParameters)
{
    while (1) {
	int total_b_field = 0;
	for (int i = 0; i < 100; i++) {
		total_b_field += hall_sens_read();
	        vTaskDelay(10 / portTICK_PERIOD_MS);
	}
        //printf("ESP32 onchip Temperature = %d\n", temprature_sens_read());
        printf("%s B_field: %d\n", tag, total_b_field /1000);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}*/

static int
callback_buttons(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{

	char TAG[50] = "[buttons-protocol]";
        esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);
	struct per_session_data__buttons *pss =
			(struct per_session_data__buttons *)user;
	struct per_vhost_data__buttons *vhd =
			(struct per_vhost_data__buttons *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));
	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	char button_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",TAG);
		xTaskCreate(adc2task, "adc2task", 1024*3, &vhd, 10, NULL);
		//xTaskCreate(read_sens_task, "read_sens_task", 1024*3, &vhd, 10, NULL);

		// initialize ADC
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_config_channel_atten(BUTTON1,ADC_ATTEN_11db);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__buttons));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_buttons, DUMB_PERIOD, DUMB_PERIOD);
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

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] button1_sum: %d\n",button1_sum);
		if (button1_sum < 100000 && button2_sum < 100000 && button3_sum < 100000 && button4_sum < 100000)
			break;
		snprintf(button1_str, 10, "%d",button1_sum);
		snprintf(button2_str, 10, "%d",button2_sum);
		snprintf(button3_str, 10, "%d",button3_sum);
		snprintf(button4_str, 10, "%d",button4_sum);
                strcpy(button_value_str, "{");
	                strcat(button_value_str, "\"mac\":\"");
			strcat(button_value_str,mac_str);
                strcat(button_value_str, "\",");
	                strcat(button_value_str, "\"button1\":");
			strcat(button_value_str,button1_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button2\":");
			strcat(button_value_str,button2_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button3\":");
			strcat(button_value_str,button3_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button4\":");
			strcat(button_value_str,button4_str);
                strcat(button_value_str, ",");
	       	        strcat(button_value_str, "\"token\":\"");
			strcat(button_value_str,token);
		strcat(button_value_str,"\"}");
		n = lws_snprintf((char *)p, sizeof(button_value_str) - LWS_PRE, "%s", button_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",TAG,button_value_str);
		} else
			printf("%s %s\n",TAG,button_value_str);
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 6)
			break;
		printf("%s %s\n",TAG,(const char *)in);
		if (strcmp((const char *)in, "reset\n") == 0)
			pss->number = 0;
		if (strcmp((const char *)in, "closeme\n") == 0) {
			lwsl_notice("dumb_inc: closing as requested\n");
			lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
					 (unsigned char *)"seeya", 5);
			return -1;
		}
		break;

	default:
	   	printf("callback_buttons: %d\n",reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_BUTTONS \
	{ \
		"buttons-protocol", \
		callback_buttons, \
		sizeof(struct per_session_data__buttons), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_buttons
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_buttons(struct lws_context *context,
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
destroy_protocol_buttons(struct lws_context *context)
{
	return 0;
}

#endif
