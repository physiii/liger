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
#include <driver/dac.h>

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define MIC_CH    16

char mac_str[20];
int AUDIO_SAMPLE_SIZE = 100;
int AUDIO_BUFFER = 255;
int value[SAMPLE_SIZE];
int audio_value;
char temp_str[50];
int audio_sum = 0;
char audio_command[100];
bool audio_linked = false;

struct per_vhost_data__audio {
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

struct per_session_data__audio {
	int number;
	int value;
};

//extern int value[1024];

static void
uv_timeout_cb_audio(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__audio *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__audio, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

void audio_task(struct per_vhost_data__audio *vhd)
{
	while(1){
		audio_sum = 0;
		for (int i = 0; i < AUDIO_BUFFER; i++) {
			dac_out_voltage(DAC_CHANNEL_1, i);
			//value[i] = gpio_get_level(MIC_CH);
			//audio_sum+=value[i];
   			vTaskDelay(1/portTICK_PERIOD_MS);
			i++;
		}

		for (int i = 0; i < AUDIO_BUFFER; i++) {
			dac_out_voltage(DAC_CHANNEL_1, 255-i);
			//value[i] = gpio_get_level(MIC_CH);
			//audio_sum+=value[i];
   			vTaskDelay(1/portTICK_PERIOD_MS);
			i++;
		}
		//vTaskDelay(100/portTICK_PERIOD_MS);
		//printf("audio_sum: %d\n",audio_sum);
	}
}

static int
callback_audio(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[audio-protocol]";

	struct per_session_data__audio *pss =
			(struct per_session_data__audio *)user;
	struct per_vhost_data__audio *vhd =
			(struct per_vhost_data__audio *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	char audio_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		xTaskCreate(audio_task, "audio_task", 1024*3, &vhd, 10, NULL);
		// initialize ADC
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_config_channel_atten(MIC_CH,ADC_ATTEN_11db);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__audio));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_audio, DUMB_PERIOD, DUMB_PERIOD);
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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] audio_sum: %d\n",audio_sum);
		if (!token_received) break;
		if (!audio_linked) {
        	        strcpy(audio_value_str, "{\"mac\":\"");
			strcat(audio_value_str,mac_str);
        	        strcat(audio_value_str, "\",\"device_type\":[\"room_sensor\"]");
        	        strcat(audio_value_str, ",\"cmd\":\"link\"");
        	        strcat(audio_value_str, ",\"token\":\"");
        	        strcat(audio_value_str, token);
        	        strcat(audio_value_str, "\"");
			strcat(audio_value_str,"}");
			n = lws_snprintf((char *)p, sizeof(audio_value_str) - LWS_PRE, "%s", audio_value_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n) 
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,audio_value_str);
			}
			break;
		}

		if (audio_sum < 100) break;

		snprintf(temp_str, 10, "%d",audio_sum);
                strcpy(audio_value_str, "{\"mac\":\"");
		strcat(audio_value_str,mac_str);
                strcat(audio_value_str, "\",\"value\":");
                strcat(audio_value_str, temp_str);
                strcat(audio_value_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(audio_value_str, ",\"cmd\":\"audio\"");
                strcat(audio_value_str, ",\"token\":\"");
                strcat(audio_value_str, token);
                strcat(audio_value_str, "\"");
		strcat(audio_value_str,"}");

		n = lws_snprintf((char *)p, sizeof(audio_value_str) - LWS_PRE, "%s", audio_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",tag,audio_value_str);
		} else
			printf("%s %s\n",tag,audio_value_str);
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;

		sprintf(audio_command,"%s",(const char *)in);
		printf("%s %s\n", tag, audio_command);
		if (strcmp(audio_command,"link")) {
			printf("%s linked!\n", tag);
			audio_linked = true;
		}

		if (!strcmp(audio_command,"threshold")) {
			printf("%s adjusting threshold!\n", tag);
			light1_value = 100;
			light_on(LIGHT1,light1_value);
		}
		break;

	default:
	   	printf("%s callback_audio: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_AUDIO \
	{ \
		"audio-protocol", \
		callback_audio, \
		sizeof(struct per_session_data__audio), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_audio
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_audio(struct lws_context *context,
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
destroy_protocol_audio(struct lws_context *context)
{
	return 0;
}

#endif
