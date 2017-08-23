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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"


static const char* TAG = "[buttons-protocol]";

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

static bool s_pad_activated[TOUCH_PAD_MAX];
bool touch_activated = false;
/*
  Read values sensed at all available touch pads.
  Use half of read value as the threshold
  to trigger interrupt when the pad is touched.
  Note: this routine demonstrates a simple way
  to configure activation threshold for the touch pads.
  Do not touch any pads when this routine
  is running (on application start).
 */
static void tp_example_set_thresholds(void)
{
    uint16_t touch_value;
    for (int i=0; i<TOUCH_PAD_MAX; i++) {
        ESP_ERROR_CHECK(touch_pad_read(i, &touch_value));
        ESP_ERROR_CHECK(touch_pad_config(i, touch_value/4));
    }
}

/*
  Check if any of touch pads has been activated
  by reading a table updated by rtc_intr()
  If so, then print it out on a serial monitor.
  Clear related entry in the table afterwards
 */
char button_str[250] = "";
static void tp_example_read_task(void *pvParameter)
{
    static int show_message;
    while (1) {
        for (int i=0; i<TOUCH_PAD_MAX; i++) {
            if (s_pad_activated[i] == true) {
                //printf("T%d activated!\n", i);
                // Wait a while for the pad being released
                vTaskDelay(200 / portTICK_PERIOD_MS);
                // Clear information on pad activation
                s_pad_activated[i] = false;
                // Reset the counter triggering a message
                // that application is running
                show_message = 1;
            }
        }
        // If no pad is touched, every couple of seconds, show a message
        // that application is running
        if (show_message++ % 500 == 0) {
            //printf("Waiting for any pad being touched...\n");
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
static void tp_example_rtc_intr(void * arg)
{
    uint32_t pad_intr = READ_PERI_REG(SENS_SAR_TOUCH_CTRL2_REG) & 0x3ff;
    uint32_t rtc_intr = READ_PERI_REG(RTC_CNTL_INT_ST_REG);
    //clear interrupt
    WRITE_PERI_REG(RTC_CNTL_INT_CLR_REG, rtc_intr);
    SET_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL2_REG, SENS_TOUCH_MEAS_EN_CLR);

    if (rtc_intr & RTC_CNTL_TOUCH_INT_ST) {
        for (int i = 0; i < TOUCH_PAD_MAX; i++) {
            if ((pad_intr >> i) & 0x01) {
                s_pad_activated[i] = true;
            }
        }
    }
}

static int
callback_buttons(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char TAG[50] = "[buttons-protocol]";
	        // Initialize touch pad peripheral
	        ESP_LOGI(TAG, "Initializing touch pad");
	        touch_pad_init();
	        tp_example_set_thresholds();
	        touch_pad_isr_handler_register(tp_example_rtc_intr, NULL, 0, NULL);
	        // Start a task to show what pads have been touched
	        xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);

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
	int n, m;

	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:

		printf("%s initialize\n",TAG);

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
		printf("[LWS_CALLBACK_CLIENT_WRITEABLE]\n");
		/*touch_activated = false;
		char temp_str[100];
		char button_num[100] = "";
		strcpy(temp_str,"[");
	        for (int i=0; i<TOUCH_PAD_MAX; i++) {
	            if (s_pad_activated[i] == true) {
	                sprintf(button_num,"%d", i);
			strcat(temp_str,button_num);
			strcat(temp_str,",");
			touch_activated = true;
	            }
        	}
		if (strlen(temp_str) > 1) {
		    temp_str[strlen(temp_str)-1] = 0;
		}
		strcat(temp_str,"]");
		if (touch_activated == false) break;
                strcpy(button_str, "{\"front_button\":");
		strcat(button_str,temp_str);
                strcat(button_str, ",\"mac\":\"");
		strcat(button_str,mac_str);
                strcat(button_str, "\",\"token\":\"");
		strcat(button_str,token);
		strcat(button_str,"\"}");
		printf("%s %s\n",TAG,button_str);
		n = lws_snprintf((char *)p, sizeof("TEST!") - LWS_PRE, "%s", "TEST!");
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",TAG,button_str);
		} else
			printf("%s %s\n",TAG,button_str);*/
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
	   	printf("%s callback_buttons: %d\n",TAG,reason);
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
