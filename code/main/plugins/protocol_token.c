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
#include "esp_partition.h"
#include "nvs_flash.h"
#include "nvs.h"

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define MIC_CHANNEL (4)
#define SAMPLE_SIZE (128)
#define SAMPLE_RATE (44100)
char temp_str[50];
bool token_received = false;
bool request_sent = false;
uint8_t mac[6];
char mac_str[20];
char token[512];
char previous_token[256];

struct per_vhost_data__token {
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

struct per_session_data__token {
	int number;
	int value;
};

static void
uv_timeout_cb_token(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__token *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__token, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}


void nvs_test()
{
    char tag[50] = "[token-protocol]";
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        const esp_partition_t* nvs_partition = esp_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        assert(nvs_partition && "partition table must have an NVS partition");
        ESP_ERROR_CHECK( esp_partition_erase_range(nvs_partition, 0, nvs_partition->size) );
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {

        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS

	size_t size;
        err = nvs_get_str(my_handle, "token", previous_token, &size);
        switch (err) {
            case ESP_OK:
                //printf(tag,"Done\n");
                printf("%s found token: %s\n", tag, previous_token);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%d) reading!\n", err);
        }

        err = nvs_set_str(my_handle, "token", token);

        if (err == ESP_OK) {
	  //printf("token: %s\n",token);
	}
	else {
	  printf("Failed!\n");
	}

        err = nvs_commit(my_handle);
        if (err == ESP_OK) {
	  printf("%s stored token: %s\n",tag,token);
	}
	else {
	  printf("Failed!\n");
	}

        // Close
        nvs_close(my_handle);
    }
}


static int
callback_token(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[token-protocol]";
        esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);
	struct per_session_data__token *pss =
			(struct per_session_data__token *)user;
	struct per_vhost_data__token *vhd =
			(struct per_vhost_data__token *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	char token_req_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__token));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_token, DUMB_PERIOD, DUMB_PERIOD);
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
		if (token_received) break;
		if (request_sent) break;

                strcpy(token_req_str, "{\"mac\":\"");
		strcat(token_req_str,mac_str);
                strcat(token_req_str, "\",\"cmd\":\"token_request\"");
                strcat(token_req_str, ",\"device_type\":[\"room_sensor\"]");
		strcat(token_req_str,"}");
		n = lws_snprintf((char *)p, sizeof(token_req_str) - LWS_PRE, "%s", token_req_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			request_sent = true;
			printf("%s %s\n",tag,token_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 6)
			break;
		printf("%s %s",tag,(const char *)in);
		sprintf(token,"%s",(const char *)in);
		nvs_test();
		token_received = true;
		break;

	default:
	   	printf("%s callback_token: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_TOKEN \
	{ \
		"token-protocol", \
		callback_token, \
		sizeof(struct per_session_data__token), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_token
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_token(struct lws_context *context,
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
destroy_protocol_token(struct lws_context *context)
{
	return 0;
}

#endif
