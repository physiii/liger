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
};

struct per_session_data__microphone {
	int number;
};

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


#define MIC_CHANNEL (4)

/*void adc1task(struct lws * wsi)
{
  // initialize ADC
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(MIC_CHANNEL,ADC_ATTEN_11db);
  int n, m;

  // connect to ws server
  struct lws_client_connect_info i;
  struct lws_context *context = lws_get_context(wsi);
  memset(&i, 0, sizeof(i));
  i.address = "192.168.0.2";
  i.host = "192.168.0.2:4000";
  i.origin = NULL;
  i.port = 4000;
  i.context = context;
  i.ietf_version_or_minus_one = -1;
  lws_client_connect_via_info(&i);

  unsigned char buf[LWS_PRE + 20];
  unsigned char *voltage = &buf[LWS_PRE];

  while(1){
      n = lws_snprintf((char *)voltage, sizeof(buf) - LWS_PRE, "%d", adc1_get_voltage(MIC_CHANNEL));
      m = lws_write(wsi, voltage, n, LWS_WRITE_TEXT);
      if (m < n)
        printf("ERROR %d writing to socket\n", n);
      printf("adc1 value:%s\n",voltage);
      vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}*/



static int
callback_microphone(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	struct per_session_data__microphone *pss =
			(struct per_session_data__microphone *)user;
	struct per_vhost_data__microphone *vhd =
			(struct per_vhost_data__microphone *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));
	unsigned char buf[LWS_PRE + 20];
	unsigned char *p = &buf[LWS_PRE];
	int n, m;
        //pss->is_connecting = false;
   	lwsl_notice("\ncallback_microphone: %d\n",reason);
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;
	case LWS_CALLBACK_PROTOCOL_INIT:
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

	case LWS_CALLBACK_SERVER_WRITEABLE:
		n = lws_snprintf((char *)p, sizeof(buf) - LWS_PRE, "%d", pss->number++);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			return -1;
		}
		break;

	case LWS_CALLBACK_RECEIVE:
		if (len < 6)
			break;
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
		break;
	}
        //xTaskCreate(adc1task, "adc1task", 1024*3, wsi, 10, NULL);
	return 0;
}

#define LWS_PLUGIN_PROTOCOL_MICROPHONE \
	{ \
		"microphone-protocol", \
		callback_microphone, \
		sizeof(struct per_session_data__microphone), \
		10, /* rx buf size must be >= permessage-deflate rx size */ \
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
