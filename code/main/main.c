/*
 * Copyright (C) 2017 Andy Payne <physiphile@gmail.com>
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
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.	So unlike the library itself, they are licensed
 * Public Domain.
 *
 */
#include <libwebsockets.h>
#include <nvs_flash.h>

#include "plugins/protocol_token.c"
#include "plugins/protocol_LED.c"
#include "plugins/protocol_buttons.c"
//#include "plugins/protocol_speaker.c"
#include "plugins/protocol_motion.c"
//#include "plugins/protocol_audio.c"
#include "plugins/protocol_ota.c"
//#include "plugins/protocol_climate.c"
//#include "plugins/protocol_update.c"
#include "plugins/protocol_esp32_lws_ota.c"
#include "plugins/protocol_esp32_lws_reboot_to_factory.c"

static const struct lws_protocols protocols_station[] = {
	{
		"http-only",
		lws_callback_http_dummy,
		0,
		1024, 0, NULL, 900
	},
	LWS_PLUGIN_PROTOCOL_TOKEN,
	LWS_PLUGIN_PROTOCOL_BUTTONS,
	LWS_PLUGIN_PROTOCOL_LED,
	//LWS_PLUGIN_PROTOCOL_CLIMATE,
	//LWS_PLUGIN_PROTOCOL_SPEAKER,
	LWS_PLUGIN_PROTOCOL_MOTION,
	//LWS_PLUGIN_PROTOCOL_AUDIO,
	LWS_PLUGIN_PROTOCOL_OTA,
	//LWS_PLUGIN_PROTOCOL_ESPLWS_SCAN,
	LWS_PLUGIN_PROTOCOL_ESPLWS_RTF,	/* helper protocol to allow reset to factory */
	{ NULL, NULL, 0, 0, 0, NULL, 0 } /* terminator */
};

static const struct lws_protocol_vhost_options pvo_headers = {
        NULL,
        NULL,
        "Keep-Alive:",
        "timeout=5, max=20",
};

/* reset to factory mount */
static const struct lws_http_mount mount_station_rtf = {
	.mountpoint		= "/esp32-rtf",
	.origin			= "esplws-rtf",
	.origin_protocol	= LWSMPRO_CALLBACK,
	.mountpoint_len		= 10,
};

/*
 * this makes a special URL "/formtest" which gets passed to
 * the "protocol-post-demo" plugin protocol for handling
 */
static const struct lws_http_mount mount_station_post = {
	.mount_next		= &mount_station_rtf,
	.mountpoint		= "/formtest",
	.origin			= "protocol-post-demo",
	.origin_protocol	= LWSMPRO_CALLBACK,
	.mountpoint_len		= 9,
};

/*
 * this serves "/station/..." in the romfs at "/" in the URL namespace
 */
static const struct lws_http_mount mount_station = {
        .mount_next		= &mount_station_post,
        .mountpoint		= "/",
        .origin			= "/station",
        .def			= "test.html",
        .origin_protocol	= LWSMPRO_FILE,
        .mountpoint_len		= 1,
};

esp_err_t event_handler(void *ctx, system_event_t *event)
{
	// deal with your own user events here first 

	return lws_esp32_event_passthru(ctx, event);
}

/*
 * This is called when the user asks to "Identify physical device"
 * he is configuring, by pressing the Identify button on the AP
 * setup page for the device.
 *
 * It should do something device-specific that
 * makes it easy to identify which physical device is being
 * addressed, like flash an LED on the device on a timer for a
 * few seconds.
 */
void
lws_esp32_identify_physical_device(void)
{
	lwsl_notice("%s\n", __func__);
}

void lws_esp32_leds_timer_cb(TimerHandle_t th)
{
}

static TimerHandle_t flash_timer;
static void flash_timer_cb(TimerHandle_t t)
{
	//flashes++;
	//lwsl_notice("timer....\n");
	//xTimerStop(flash_timer, 0);
	/*if (flashes & 1)
		gpio_output_set(0, 1 << GPIO_ID, 1 << GPIO_ID, 0);
	else
		gpio_output_set(1 << GPIO_ID, 0, 1 << GPIO_ID, 0);*/
}

uint8_t mac[6];
char mac_str[20];
void app_main(void)
{
        esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);

	nvs_flash_init();
	lws_esp32_wlan_config();
	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL));
	lws_esp32_wlan_start_station();

	static struct lws_context_creation_info info;
	static struct lws_client_connect_info i;
	static struct lws_client_connect_info j;
	struct lws_context *context;
        struct lws *wsi;
        struct lws *wsi2;

	memset(&info, 0, sizeof(info));
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols_station;
	info.gid = -1;
	info.uid = -1;
	info.ws_ping_pong_interval = 10;
	context = lws_esp32_init(&info);

	memset(&i, 0, sizeof i);
	i.address = "pyfi.org";
        i.port = 4000;
	i.ssl_connection = 0;
	i.host = i.address;
	i.origin = i.host;
        i.ietf_version_or_minus_one = -1;
	i.pwsi = &wsi;
	i.context = context;

	// ------------------ //
	// initiate protocols //
	// ------------------ //
	i.protocol = "token-protocol";
	i.path = "/tokens";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	i.protocol = "buttons-protocol";
	i.path = "/buttons";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	i.protocol = "LED-protocol";
	i.path = "/LED";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	i.protocol = "motion-protocol";
	i.path = "/motion";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	i.protocol = "audio-protocol";
	i.path = "/audio";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	/*i.protocol = "climate-protocol";
	i.path = "/climate";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }*/

	i.protocol = "ota-protocol";
	i.path = "/update";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }

	/*i.protocol = "esplws-scan";
        wsi = lws_client_connect_via_info(&i);
        while (!wsi) {
	        wsi = lws_client_connect_via_info(&i);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
        }*/

	// ----------------- //
	// service protocols //
	// ----------------- //
	while (!lws_service(context, 500)) {
		taskYIELD();
	}
        return 0;
}
