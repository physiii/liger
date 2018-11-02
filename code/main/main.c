/*
 * Example ESP32 app code using Libwebsockets
 *
 * Copyright (C) 2017 Andy Green <andy@warmcat.com>
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
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "cJSON.h"

#include "../components/libwebsockets/plugins/protocol_lws_status.c"
#include <protocol_esp32_lws_reboot_to_factory.c>

struct lws *wsi_token;
int wsi_connect = 1;
unsigned int rl_token = 0;
char token[512];
char device_id[100];
bool token_received = false;
bool reconnect_with_token = false;
static struct lws_client_connect_info i;
char server_address[100] = "dev.pyfi.org";
bool use_ssl = true;

//needs to go in headers

int set_switch(int);

/////////////////

#include "services/storage.c"
#include "plugins/protocol_wss.c"
#include "services/button.c"
//#include "services/motion.c"
#include "services/LED.c"
#include "services/dimmer.c"
#include "services/scheduler.c"
/*#include "services/audio.c
#include "services/contact-sensor.c"*/

static const struct lws_protocols protocols_station[] = {
	{
		"http-only",
		lws_callback_http_dummy,
		0,
		1024, 0, NULL, 900
	},
	//LWS_PLUGIN_PROTOCOL_TOKEN, /* demo... */
	//LWS_PLUGIN_PROTOCOL_TEST_CLIENT, /* demo... */
	LWS_PLUGIN_PROTOCOL_WSS, /* demo... */
	LWS_PLUGIN_PROTOCOL_LWS_STATUS,	    /* plugin protocol */
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

/*
 * this serves "/secret/" in the romfs at "/secret" in the URL namespace
 * it requires basic auth, which for the demo is set to literally user / password
 * This is just demoing how to do basic auth.
 *
 * See below how the password is set
 */
static const struct lws_http_mount mount_station_needs_auth = {
        .mount_next		= &mount_station,
        .mountpoint		= "/secret",
        .origin			= "/secret",
        .def			= "index.html",
        .origin_protocol	= LWSMPRO_FILE,
        .mountpoint_len		= 7,

	.basic_auth_login_file	= "lwsdemoba", /* esp32 nvs realm to use */
};

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


bool got_ip = false;
esp_err_t event_handler(void *ctx, system_event_t *event)
{
	/* deal with your own user events here first */

    switch(event->event_id) {


        case SYSTEM_EVENT_STA_START:
            printf("SYSTEM_EVENT_STA_START\n");
    //do not actually connect in test case
            //;
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            printf("got ip: %s\n",
            ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
	    			got_ip = true;
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            printf("SYSTEM_EVENT_STA_DISCONNECTED\n");
	    			got_ip = false;
            //TEST_ESP_OK(esp_wifi_connect());
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
	    			wsi_connect = 1;
            //TEST_ESP_OK(esp_wifi_connect());
            break;

        default:
 	    printf("ev_handle_called.\n");
            break;
    }
    //return ESP_OK;

	return lws_esp32_event_passthru(ctx, event);
}

static int ratelimit_connects(unsigned int *last, unsigned int secs)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	if (tv.tv_sec - (*last) < secs)
		return 0;

	*last = tv.tv_sec;

	return 1;
}

int
connect_client(struct lws_client_connect_info i)
{
	printf("connecting to client...\n");
	return lws_client_connect_via_info(&i);
}

void app_main(void)
{

	//strcpy(device_id,"25dc4876-d1e2-4d6e-ba4f-fba81992c888");
	static struct lws_context_creation_info info;
	struct lws_context *context;
	struct lws_vhost *vh;
	nvs_handle nvh;

	lws_esp32_set_creation_defaults(&info);

	info.port = 443;
	info.fd_limit_per_thread = 12;
	info.max_http_header_pool = 12;
	info.max_http_header_data = 512;
	info.pt_serv_buf_size = 900;
	info.keepalive_timeout = 5;
	info.simultaneous_ssl_restriction = 12;
	info.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS |
		       LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

	info.ssl_cert_filepath = "ap-cert.pem";
	info.ssl_private_key_filepath = "ap-key.pem";

	info.vhost_name = "station";
	info.protocols = protocols_station;
	info.mounts = &mount_station_needs_auth;
	info.headers = &pvo_headers;

	nvs_flash_init();
	lws_esp32_wlan_config();

	/*
	 * set the basic auth user:password used for /secret/... urls
	 * normally you'd just do this once at setup-time or if the
	 * password was changed.  If you don't use basic auth on your
	 * site, no need for this.
	 */
	if (!nvs_open("lwsdemoba", NVS_READWRITE, &nvh)) {
		nvs_set_str(nvh, "user", "password");
		nvs_commit(nvh);
		nvs_close(nvh);
	}

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL));

	lws_esp32_wlan_start_station();
	context = lws_esp32_init(&info, &vh);

	memset(&i, 0, sizeof i);
	i.address = server_address;
	i.port = 5050;
	i.host = i.address;
	i.origin = i.host;
	i.ietf_version_or_minus_one = -1;
	i.context = context;
	i.protocol = "wss-protocol";
	i.pwsi = &wsi_token;
	i.path = "/device-relay";
	if (use_ssl) {
		i.ssl_connection = LCCSCF_USE_SSL;
		i.ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
	}
	strcpy(token,get_char("token"));
	printf("pulled token from storage: %s\n", token);

	buttons_main();
  //LED_main();
	//dimmer_main();
	//scheduler_main();
	//motion_main();
	//audio_main();
	//contact_main();

	bool send_load_event = true;
	char load_message[500];

	while (1) {

		if (send_load_event) {

			sprintf(load_message,""
			"{\"event_type\":\"load\","
			" \"payload\":{\"services\":["
			"{\"type\":\"button\","
			"\"state\":{\"dpad\":0}},"
			"{\"type\":\"motion\","
			"\"state\":{\"channel_0\":0}},"
			"{\"type\":\"dimmer\","
			"\"state\":{\"level\":0, \"on\":false}},"
			"{\"type\":\"LED\","
			"\"state\":{\"rgb\":[0,0,0]},"
			"\"id\":1}"
			"]}}");
			printf("load_mesage %s\n",load_message);
			strcpy(wss_data_out,load_message);
			send_load_event = false;
			wss_data_out_ready = true;
		}

		if (buttons_service_message_ready && !wss_data_out_ready) {
			strcpy(wss_data_out,buttons_service_message);
			buttons_service_message_ready = false;
			wss_data_out_ready = true;
		}

		/*if (contact_service_message_ready && !wss_data_out_ready) {
			strcpy(wss_data_out,contact_service_message);
			contact_service_message_ready = false;
			wss_data_out_ready = true;
		}*/

		if (wsi_connect && got_ip && ratelimit_connects(&rl_token, 4u)) {
			wsi_connect = 0;
			lws_client_connect_via_info(&i);
			//connect_client(i);
		}

		if (wsi_connect) {
    	set_pixel_by_index(0, 0, 0, 0, 1);
			vTaskDelay(300 / portTICK_RATE_MS);
    	set_pixel_by_index(0, 0, 0, 255, 1);
		} else {
			set_pixel_by_index(0, 0, 255, 0, 1);
		}

		lws_service(context, 500);
		taskYIELD();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
