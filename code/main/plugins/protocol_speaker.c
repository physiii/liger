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
#include <driver/dac.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"

struct per_vhost_data__speaker {
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

struct per_session_data__speaker {
	int number;
	int value;
};

static void
uv_timeout_cb_speaker(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__speaker *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__speaker, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

void speaker_task(struct per_vhost_data__motion *vhd)
{
        static const char* TAG = "[speaker-protocol]";
	bool push_out = true;
	/*adc_timer = xTimerCreate("x", pdMS_TO_TICKS(1 / SAMPLE_RATE), 1, NULL,
		(TimerCallbackFunction_t)adc_timer_cb);
	xTimerStart(adc_timer, 0);*/
	while(1){
		if (push_out) {
          		dac_output_voltage(DAC_CHANNEL_1, 1024);
			push_out = false;
		} else {
          		dac_output_voltage(DAC_CHANNEL_1, 0);
			push_out = true;
		}
		vTaskDelay(10/portTICK_PERIOD_MS);
		//printf("%s speaker active\n",TAG);
	}
}

static int
callback_speaker(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
        static const char* TAG = "[speaker-protocol]";
        dac_output_enable(DAC_CHANNEL_1);
        xTaskCreate(&speaker_task, "speaker_task", 2048, NULL, 5, NULL);

        esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);
	struct per_session_data__speaker *pss =
			(struct per_session_data__speaker *)user;
	struct per_vhost_data__speaker *vhd =
			(struct per_vhost_data__speaker *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));
	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	int n, m;

	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:

		printf("%s initialize\n",TAG);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__speaker));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			        uv_timeout_cb_speaker, DUMB_PERIOD, DUMB_PERIOD);
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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] button_up_sum: %d\n",button_up_sum);
		n = lws_snprintf((char *)p, sizeof("TOUCH!") - LWS_PRE, "%s", "TOUCH!");
		/*m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",TAG,"TOUCH!");
		} else {
			//printf("%s %s\n",TAG,button_value_str);
		}*/
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 6)
			break;
		printf("%s[LWS_CALLBACK_CLIENT_RECEIVE] %s\n",TAG,(const char *)in);
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
	   	printf("%s callback_speaker: %d\n",TAG,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_SPEAKER \
	{ \
		"speaker-protocol", \
		callback_speaker, \
		sizeof(struct per_session_data__speaker), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_speaker
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_speaker(struct lws_context *context,
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
destroy_protocol_speaker(struct lws_context *context)
{
	return 0;
}

#endif
