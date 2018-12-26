#include <libwebsockets.h>
#include <nvs_flash.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "cJSON.h"

#include "../components/libwebsockets/plugins/protocol_lws_status.c"
#include <protocol_esp32_lws_reboot_to_factory.c>

char server_address[20] = "dev.pyfi.org";
int port = 443;
bool use_ssl = true;

int DISCONNECTED = 0;
int CONNECTING   = 1;
int CONNECTED    = 2;
int utility_server_status = 0;
int relay_status = 0;
int previous_relay_status = 0;

struct lws *wsi_token;
int wsi_connect = 1;
unsigned int rl_token = 0;
unsigned int rl_ping = 0;
unsigned int rl_device_id = 0;
char token[1000];
char device_id[100];
bool start_service_loop = false;
bool token_received = false;
bool reconnect_with_token = false;
static struct lws_client_connect_info relay;
static struct lws_client_connect_info utility_server;
struct lws_vhost *vh;
bool sent_load_event = false;
char load_message[500];
static struct lws_context_creation_info info;
struct lws_context *context;
cJSON *payload = NULL;
cJSON *utility_payload = NULL;
cJSON *dimmer_payload = NULL;
cJSON *LED_payload = NULL;
cJSON *schedule_payload = NULL;
int current_time = 0;
bool got_ip = false;

//needs to go in headers
int set_switch(int);
void debounce_pir();
static int ratelimit_connects(unsigned int *last, unsigned int secs);

#include "services/storage.c"
#include "services/LED.c"
#include "plugins/protocol_relay.c"
#include "plugins/protocol_utility.c"
#include "services/button.c"
#include "services/dimmer.c"
#include "services/motion.c"
#include "services/scheduler.c"
/*#include "services/audio.c*/

static const struct lws_protocols protocols_station[] = {
	{
		"http-only",
		lws_callback_http_dummy,
		0,
		1024, 0, NULL, 900
	},
	LWS_PLUGIN_PROTOCOL_WSS,
	LWS_PLUGIN_PROTOCOL_UTILITY,
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

void lws_esp32_leds_timer_cb(TimerHandle_t th) {}

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
void lws_esp32_identify_physical_device(void) {
	lwsl_notice("%s\n", __func__);
}

esp_err_t event_handler(void *ctx, system_event_t *event) {
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
            printf("LWS_CALLBACK_CLIENT_CONNECTION_ERROR %d\n",LWS_CALLBACK_CLIENT_CLOSED_CNT);
	    			//wsi_connect = 1;
            //TEST_ESP_OK(esp_wifi_connect());
            break;

        default:
 	    printf("ev_handle_called.\n");
            break;
    }
    //return ESP_OK;

	return lws_esp32_event_passthru(ctx, event);
}

static int ratelimit_connects(unsigned int *last, unsigned int secs) {
	struct timeval tv;

	gettimeofday(&tv, NULL);

	if (tv.tv_sec - (*last) < secs)
		return 0;

	*last = tv.tv_sec;

	return 1;
}

int load_device_id() {
	strcpy(device_id,get_char("device_id"));

	if (strcmp(device_id,"")==0) {
		memset(&utility_server, 0, sizeof utility_server);
		utility_server.address = server_address;
		utility_server.port = port;
		utility_server.host = utility_server.address;
		utility_server.origin = utility_server.host;
		utility_server.ietf_version_or_minus_one = -1;
		utility_server.context = context;
		utility_server.protocol = "utility-protocol";
		utility_server.pwsi = &wsi_token;
		utility_server.path = "/utilities";
		if (use_ssl) {
			utility_server.ssl_connection = LCCSCF_USE_SSL;
			utility_server.ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
		}
	}

	while (strcmp(device_id,"")==0) {
		printf("utility loop (%d)\n",cnt++);

		if (got_ip && ratelimit_connects(&rl_device_id, 5u)) {
			snprintf(utility_data_out,sizeof(utility_data_out),"{\"event_type\":\"generate-uuid\"}");
			utility_data_out_ready = true;
			printf("device_id_main...\n");
		}

		if (got_ip && utility_server_status != CONNECTED && ratelimit_connects(&rl_token, 4u)) {
			utility_server_status = CONNECTING;
			lws_client_connect_via_info(&utility_server);
			printf("connecting to %s\n",server_address);
		}

		if (got_ip && utility_server_status != DISCONNECTED) {
			lws_service(context, 1000);
		}

		vTaskDelay(1000 / portTICK_RATE_MS);
	}

	return 0;
}

void app_main(void) {
	nvs_handle nvh;

	lws_esp32_set_creation_defaults(&info);

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.vhost_name = "station";
	info.protocols = protocols_station;

	nvs_flash_init();
	lws_esp32_wlan_config();

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL));

	lws_esp32_wlan_start_station();
	context = lws_esp32_init(&info, &vh);

	buttons_main();
	LED_main();
	dimmer_main();
	schedule_main();
	motion_main();

	//store_char("token","");
	//store_char("device_id","");
	load_device_id();

	printf("Device ID: %s\n",device_id);


	strcpy(token,get_char("token"));
	if (strcmp(token,"")==0) {
		strcpy(token,device_id);
		printf("no token found, setting token as device id: %s\n", token);
	} else {
		printf("pulled token from storage: %s\n", token);
	}

	int service_context_cnt = 0;
	int cnt = 0;

	memset(&relay, 0, sizeof relay);
	relay.address = server_address;
	relay.port = port;
	relay.host = relay.address;
	relay.origin = relay.host;
	relay.ietf_version_or_minus_one = -1;
	relay.context = context;
	relay.protocol = "wss-protocol";
	relay.pwsi = &wsi_token;
	relay.path = "/device-relay";
	if (use_ssl) {
		relay.ssl_connection = LCCSCF_USE_SSL;
		relay.ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
	}

	snprintf(load_message,sizeof(load_message),""
	"{\"event_type\":\"load\","
	" \"payload\":{\"services\":["
	"{\"type\":\"dimmer\","
	"\"state\":{\"level\":0, \"on\":false},\"id\":\"dimmer_1\"},"
	"{\"type\":\"LED\","
	"\"state\":{\"rgb\":[0,0,0]},"
	"\"id\":1}"
	"]}}");
	strcpy(wss_data_out,load_message);
	wss_data_out_ready = true;
	printf("load_mesage %s\n",load_message);

	while (1) {
		//printf("main loop (%d)\n",cnt++);

		if (relay_status == DISCONNECTED) {
			setLED(0, 0, 0);
			vTaskDelay(300 / portTICK_RATE_MS);
			setLED(0, 0, 255);
		}

		if (buttons_service_message_ready && !wss_data_out_ready) {
			strcpy(wss_data_out,buttons_service_message);
			buttons_service_message_ready = false;
			wss_data_out_ready = true;
		}


		if (got_ip
			&& relay_status == CONNECTED
			&& ratelimit_connects(&rl_ping, 60u)
			&& strcmp(wss_data_out,"")==0) {
			snprintf(wss_data_out,sizeof(wss_data_out),"{\"event_type\":\"ping\"}");
			wss_data_out_ready = true;
		}

		// if (contact_service_message_ready && !wss_data_out_ready) {
		// 	strcpy(wss_data_out,contact_service_message);
		// 	contact_service_message_ready = false;
		// 	wss_data_out_ready = true;
		// }


		if (got_ip && relay_status == DISCONNECTED && ratelimit_connects(&rl_token, 4u)) {
			relay_status = CONNECTED;
			lws_client_connect_via_info(&relay);
			printf("connecting to %s\n",server_address);
		}

		if (got_ip && relay_status != DISCONNECTED) {
				lws_service(context, 1000);
		}

		previous_relay_status = relay_status;
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}
