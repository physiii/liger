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

#define PIR    16

uint8_t mac[6];
char mac_str[20];
int PIR_SAMPLE_SIZE = 100;
int value[SAMPLE_SIZE];
int pir_value;
char temp_str[50];
int pir_sum = 0;
char motion_command[100];
bool motion_linked = false;

struct per_vhost_data__motion {
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

struct per_session_data__motion {
	int number;
	int value;
};

//extern int value[1024];

static void
uv_timeout_cb_motion(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__motion *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__motion, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

void pir_task(struct per_vhost_data__motion *vhd)
{
	while(1){
		pir_sum = 0;
		for (int i = 0; i < SAMPLE_SIZE; i++) {
			value[i] = gpio_get_level(PIR);
			pir_sum+=value[i];
   			vTaskDelay(10/portTICK_PERIOD_MS);
		}
		//vTaskDelay(100/portTICK_PERIOD_MS);
		//printf("pir_sum: %d\n",pir_sum);
	}
}

static int
callback_motion(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[motion-protocol]";
        /*esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);*/
	struct per_session_data__motion *pss =
			(struct per_session_data__motion *)user;
	struct per_vhost_data__motion *vhd =
			(struct per_vhost_data__motion *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	char pir_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		xTaskCreate(pir_task, "pir_task", 1024*3, &vhd, 10, NULL);
		// initialize ADC
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_config_channel_atten(PIR,ADC_ATTEN_11db);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__motion));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_motion, DUMB_PERIOD, DUMB_PERIOD);
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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] pir_sum: %d\n",pir_sum);
		if (!token_received) break;
		if (!motion_linked) {
        	        strcpy(pir_value_str, "{\"mac\":\"");
			strcat(pir_value_str,mac_str);
        	        strcat(pir_value_str, "\",\"device_type\":[\"room_sensor\"]");
        	        strcat(pir_value_str, ",\"cmd\":\"link\"");
        	        strcat(pir_value_str, ",\"token\":\"");
        	        strcat(pir_value_str, token);
        	        strcat(pir_value_str, "\"");
			strcat(pir_value_str,"}");
			n = lws_snprintf((char *)p, sizeof(pir_value_str) - LWS_PRE, "%s", pir_value_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n) 
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,pir_value_str);
			}
			break;
		}

		if (pir_sum < 100) break;

		snprintf(temp_str, 10, "%d",pir_sum);
                strcpy(pir_value_str, "{\"mac\":\"");
		strcat(pir_value_str,mac_str);
                strcat(pir_value_str, "\",\"value\":");
                strcat(pir_value_str, temp_str);
                strcat(pir_value_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(pir_value_str, ",\"cmd\":\"motion\"");
                strcat(pir_value_str, ",\"token\":\"");
                strcat(pir_value_str, token);
                strcat(pir_value_str, "\"");
		strcat(pir_value_str,"}");

		n = lws_snprintf((char *)p, sizeof(pir_value_str) - LWS_PRE, "%s", pir_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",tag,pir_value_str);
		} else
			printf("%s %s\n",tag,pir_value_str);
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;

		sprintf(motion_command,"%s",(const char *)in);
		printf("%s %s\n", tag, motion_command);
		if (strcmp(motion_command,"link")) {
			printf("%s linked!\n", tag);
			motion_linked = true;
		}

		if (!strcmp(motion_command,"threshold")) {
			printf("%s adjusting threshold!\n", tag);
			light1_value = 100;
			light_on(LIGHT1,light1_value);
		}
		break;

	default:
	   	printf("%s callback_motion: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_MOTION \
	{ \
		"motion-protocol", \
		callback_motion, \
		sizeof(struct per_session_data__motion), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_motion
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_motion(struct lws_context *context,
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
destroy_protocol_motion(struct lws_context *context)
{
	return 0;
}

#endif
