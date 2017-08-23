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


#define MIC_CHANNEL (6)
#define SAMPLE_SIZE (128)
#define SAMPLE_RATE (44100)

int value[SAMPLE_SIZE];
int button_value;
char temp_str[50];
int sum = 0;
char mic_value_str[256];

struct per_vhost_data__microphone {
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

struct per_session_data__microphone {
	int number;
	int value;
};

//extern int value[1024];

static void
uv_timeout_cb_microphone(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__microphone *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__microphone, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

static TimerHandle_t adc_timer;
static void adc_timer_cb(TimerHandle_t t)
{
	uint32_t ulCount;
	ulCount = ( uint32_t ) pvTimerGetTimerID( t );
	ulCount++;
	/*TickType_t xTimerPeriod;
	xTimerPeriod = xTimerGetPeriod( t );*/
	printf("timer count: %d\n", ulCount);
	//vTimerSetTimerID( t, ( void * ) ulCount );
	//xTimerStop(adc_timer, 0);
	/*if (flashes & 1)
		gpio_output_set(0, 1 << GPIO_ID, 1 << GPIO_ID, 0);
	else
		gpio_output_set(1 << GPIO_ID, 0, 1 << GPIO_ID, 0);*/
}

void microphone_task(struct per_vhost_data__microphone *vhd)
{
	char TAG[50] = "[microphone-protocol]";
	/*adc_timer = xTimerCreate("x", pdMS_TO_TICKS(1 / SAMPLE_RATE), 1, NULL,
		(TimerCallbackFunction_t)adc_timer_cb);
	xTimerStart(adc_timer, 0);*/
	while(1){
		//sum = adc1_get_voltage(MIC_CHANNEL);
		sum = 0;
		for (int i = 0; i < SAMPLE_SIZE; i++) {
			value[i] = adc1_get_voltage(MIC_CHANNEL);
			sum+=value[i];
		}
		/*printf("%s",TAG);
		for (int i = 0; i < sum; i++) {
			printf("-");
			i+=50;
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
		printf("[%d]\n",sum);*/
	}
}

static int
callback_microphone(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char TAG[50] = "[microphone-protocol]";

	struct per_session_data__microphone *pss =
			(struct per_session_data__microphone *)user;
	struct per_vhost_data__microphone *vhd =
			(struct per_vhost_data__microphone *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 20];
	unsigned char *p = &buf[LWS_PRE];
	char button_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",TAG);
		xTaskCreate(microphone_task, "microphone_task", 1024*3, &vhd, 10, NULL);
		// initialize ADC
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_config_channel_atten(MIC_CHANNEL,ADC_ATTEN_11db);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__microphone));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_microphone, DUMB_PERIOD, DUMB_PERIOD);
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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] sum: %d\n",sum);
		if (sum < 10000) break;
		snprintf(temp_str, 10, "%d",sum);

                strcpy(mic_value_str, "{\"mac\":\"");
		strcat(mic_value_str,mac_str);
                strcat(mic_value_str, "\",\"value\":");
                strcat(mic_value_str, temp_str);
                strcat(mic_value_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(mic_value_str, ",\"token\":\"");
                strcat(mic_value_str, token);
                strcat(mic_value_str, "\"");
		strcat(mic_value_str,"}");

		n = lws_snprintf((char *)p, sizeof(mic_value_str) - LWS_PRE, "%s", mic_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else
			printf("%s %s\n",TAG,mic_value_str);
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
	   	printf("callback_microphone: %d\n",reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_MICROPHONE \
	{ \
		"microphone-protocol", \
		callback_microphone, \
		sizeof(struct per_session_data__microphone), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_MICROPHONE
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_microphone(struct lws_context *context,
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
destroy_protocol_microphone(struct lws_context *context)
{
	return 0;
}

#endif
