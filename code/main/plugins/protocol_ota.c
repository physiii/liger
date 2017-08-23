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
#include <stdlib.h>

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

#include "nvs.h"
#include "nvs_flash.h"

/* protocol for scan updates over ws and saving wlan setup */
//#include "protocol_esp32_lws_scan.c"
/* protocol for OTA update using POST / https / browser upload */
//#include "protocol_esp32_lws_ota.c"

//#define CONFIG_LWS_OTA_SERVER_BASE_URL "downloads"
#define CONFIG_LWS_OTA_SERVER_FQDN "192.168.0.15"

#define EXAMPLE_SERVER_IP   "192.168.0.15"
#define EXAMPLE_SERVER_PORT "5000"
//#define EXAMPLE_FILENAME "/downloads/liger.bin"
#define BUFFSIZE 1024
#define TEXT_BUFFSIZE 1024

char temp_str[50];
bool ota_received = false;
bool update_linked = false;
char ota[256];
char update_value_str[256];
char update_command[100];

struct per_session_data__ota {
	int number;
	int value;
	struct per_session_data__esplws_scan *next;
	//scan_state scan_state;
	struct timeval last_send;

	struct lws_spa *spa;
	char filename[32];
	char result[LWS_PRE + 512];
	unsigned char buffer[4096];
	int result_len;
	int filename_length;
	long file_length;
	nvs_handle nvh;

	char ap_record;
	unsigned char subsequent:1;
	unsigned char changed_partway:1;
};

struct per_vhost_data__ota {
	uv_timer_t timeout_watcher;
	wifi_ap_record_t ap_records[10];
	TimerHandle_t timer, reboot_timer;
	struct per_session_data__esplws_scan *live_pss_list;
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;

	const esp_partition_t *part;
	esp_ota_handle_t otahandle;
	long file_length;
	long content_length;

	struct lws *cwsi;
	char json[1024];
	int json_len;

	uint16_t count_ap_records;
	char count_live_pss;
	unsigned char scan_ongoing:1;
	unsigned char completed_any_scan:1;
	unsigned char reboot:1;
	unsigned char changed_settings:1;
	unsigned char checked_updates:1;
	unsigned char autonomous_update:1;
	unsigned char autonomous_update_sampled:1;
};

static void
uv_timeout_cb_ota(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__ota *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__ota, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

static int
callback_ota(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[ota-protocol]";

	struct per_session_data__ota *pss =
			(struct per_session_data__ota *)user;
	struct per_vhost_data__ota *vhd =
			(struct per_vhost_data__ota *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));

	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	char ota_req_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__ota));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_ota, DUMB_PERIOD, DUMB_PERIOD);
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

		if (!update_linked) {
        	        strcpy(update_value_str, "{\"mac\":\"");
			strcat(update_value_str,mac_str);
        	        strcat(update_value_str, "\",\"device_type\":[\"room_sensor\"]");
        	        strcat(update_value_str, ",\"cmd\":\"link\"");
        	        strcat(update_value_str, ",\"token\":\"");
        	        strcat(update_value_str, token);
        	        strcat(update_value_str, "\"");
			strcat(update_value_str,"}");
			n = lws_snprintf((char *)p, sizeof(update_value_str) - LWS_PRE, "%s", update_value_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n) 
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,update_value_str);
			}
			break;
		}

		/*if (pir_sum < 100) break;

		snprintf(temp_str, 10, "%d",pir_sum);
                strcpy(update_value_str, "{\"mac\":\"");
		strcat(update_value_str,mac_str);
                strcat(update_value_str, "\",\"value\":");
                strcat(update_value_str, temp_str);
                strcat(update_value_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(update_value_str, ",\"cmd\":\"update\"");
                strcat(update_value_str, ",\"token\":\"");
                strcat(update_value_str, token);
                strcat(update_value_str, "\"");
		strcat(update_value_str,"}");

		n = lws_snprintf((char *)p, sizeof(update_value_str) - LWS_PRE, "%s", update_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",tag,update_value_str);
		} else
			printf("%s %s\n",tag,update_value_str);*/
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;

		sprintf(update_command,"%s",(const char *)in);
		printf("%s %s\n", tag, update_command);
		if (strcmp(update_command,"link")) {
			printf("%s linked!\n", tag);
			update_linked = true;
		}

		//init_ota();
		break;

	case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
		//lwsl_notice("LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ: %ld\n",
		//	    (long)len);

		if (!vhd->autonomous_update) {
			if (sizeof(vhd->json) - vhd->json_len - 1 < len)
				len = sizeof(vhd->json) - vhd->json_len - 1;
			memcpy(vhd->json + vhd->json_len, in, len);
			vhd->json_len += len;
			vhd->json[vhd->json_len] = '\0';
			break;
		}

		/* autonomous download */


		if (vhd->file_length + len > vhd->part->size) {
			lwsl_err("OTA: incoming file too large\n");
			goto abort_ota;
		}

		lwsl_debug("writing 0x%lx... 0x%lx\n",
			   vhd->part->address + vhd->file_length,
			   vhd->part->address + vhd->file_length + len);
		if (esp_ota_write(vhd->otahandle, in, len) != ESP_OK) {
			lwsl_err("OTA: Failed to write\n");
			goto abort_ota;
		}
		vhd->file_length += len;

		lws_callback_on_writable_all_protocol(vhd->context, vhd->protocol);
		break;

abort_ota:
		esp_ota_end(vhd->otahandle);
		vhd->otahandle = 0;
		vhd->autonomous_update = 0;

		return 1;

	case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
		{
			char *px = (char *)pss->buffer + LWS_PRE;
			int lenx = sizeof(pss->buffer) - LWS_PRE - 1;

			//lwsl_notice("LWS_CALLBACK_RECEIVE_CLIENT_HTTP: %d\n", len);

			if (lws_http_client_read(wsi, &px, &lenx) < 0)
				return -1;
		}
		break;

	/*case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
		lwsl_notice("LWS_CALLBACK_COMPLETED_CLIENT_HTTP\n");
		vhd->cwsi = NULL;
		if (!vhd->autonomous_update) {

			vhd->checked_updates = 1;
			puts(vhd->json);
			return -1;
		}

		// autonomous download

		lwsl_notice("auton complete\n");

		if (esp_ota_end(vhd->otahandle) != ESP_OK) {
			lwsl_err("OTA: end failed\n");
			return 1;
		}

		if (esp_ota_set_boot_partition(vhd->part) != ESP_OK) {
			lwsl_err("OTA: set boot part failed\n");
			return 1;
		}
		vhd->otahandle = 0;
		vhd->autonomous_update = 0;

		vhd->reboot_timer = xTimerCreate("x", pdMS_TO_TICKS(250), 0, vhd,
			  (TimerCallbackFunction_t)reboot_timer_cb);
			xTimerStart(vhd->reboot_timer, 0);
		return -1;*/

	case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
		lwsl_notice("LWS_CALLBACK_CLOSED_CLIENT_HTTP\n");
		break;

	case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
		/* called when our wsi user_space is going to be destroyed */
		if (pss->spa) {
			lws_spa_destroy(pss->spa);
			pss->spa = NULL;
		}
		break;

	default:
	   	printf("%s callback_ota: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_OTA \
	{ \
		"ota-protocol", \
		callback_ota, \
		sizeof(struct per_session_data__ota), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_OTA
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_ota(struct lws_context *context,
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
destroy_protocol_ota(struct lws_context *context)
{
	return 0;
}

#endif
