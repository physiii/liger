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
bool initiate_protocols = false;
bool is_connected = false;
bool token_received = false;
char device_type[30] = "room_sensor";
char device_type_str[50] = "";

#include "plugins/protocol_token.c"
#include "plugins/protocol_buttons.c"
//#include "plugins/protocol_power.c"
//#include "plugins/protocol_climate.c"

//#include "plugins/protocol_update.c"
//#include "plugins/protocol_LED.c"
//#include "plugins/protocol_speaker.c"
//#include "plugins/protocol_motion.c"
//#include "plugins/protocol_audio.c"
//#include "plugins/protocol_ota.c"

#include "plugins/protocol_esp32_lws_ota.c"
#include "../components/libwebsockets/plugins/protocol_lws_meta.c"
#include <protocol_esp32_lws_reboot_to_factory.c>

/*#define DATA_LENGTH          512
#define RW_TEST_LENGTH       127  
#define POWER_DELAY_TIME    5000 

#define I2C_EXAMPLE_MASTER_SCL_IO    19
#define I2C_EXAMPLE_MASTER_SDA_IO    18
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0
#define I2C_EXAMPLE_MASTER_FREQ_HZ   100000

#define WRITE_BIT  I2C_MASTER_WRITE
#define READ_BIT   I2C_MASTER_READ
#define ACK_CHECK_EN   0x1
#define ACK_CHECK_DIS  0x0
#define ACK_VAL    0x0
#define NACK_VAL   0x1*/

static const struct lws_protocols protocols_station[] = {
	{
		"http-only",
		lws_callback_http_dummy,
		0,
		1024, 0, NULL, 900
	},
	LWS_PLUGIN_PROTOCOL_TOKEN,
	LWS_PLUGIN_PROTOCOL_BUTTONS,
	//LWS_PLUGIN_PROTOCOL_POWER,
	//LWS_PLUGIN_PROTOCOL_CLIMATE,
	//LWS_PLUGIN_PROTOCOL_LED,
	//LWS_PLUGIN_PROTOCOL_SPEAKER,
	//LWS_PLUGIN_PROTOCOL_MOTION,
	//LWS_PLUGIN_PROTOCOL_AUDIO,
	//LWS_PLUGIN_PROTOCOL_OTA,
	//LWS_PLUGIN_PROTOCOL_ESPLWS_SCAN,
	LWS_PLUGIN_PROTOCOL_ESPLWS_RTF,	// helper protocol to allow reset to factory
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
char mac_str[20] = "init";

struct lws_vhost *vh;
static struct lws_context_creation_info info;
static struct lws_client_connect_info i;
struct lws_context *context;
struct lws *wsi_tokens, 
           *wsi_buttons, 
           *wsi_power, 
           *wsi_climate,
           *wsi_motion,
           *wsi_LED;



void init_protocols(void)
{
	//esp_err_t ESP_OK = esp_wifi_sta_get_ap_info();
	// ------------------ //
	// initiate protocols //
	// ------------------ //
	while (1) 
	{
		if (!is_connected)
		{
			wsi_tokens = NULL;
			wsi_buttons = NULL;
			wsi_motion = NULL;
			wsi_LED = NULL;
			//printf("\CONNECTING!\n");
			i.pwsi = &wsi_tokens;
			i.protocol = "token-protocol";
			i.path = "/tokens";
		        wsi_tokens = lws_client_connect_via_info(&i);
		        while (!wsi_tokens) {
			        wsi_tokens = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(5000/portTICK_PERIOD_MS);
				printf("TOKEN-PROTOCOL!\n");	
	       		}
		}
		if (initiate_protocols) 
		{
			i.pwsi = &wsi_buttons;
			i.protocol = "buttons-protocol";
			i.path = "/buttons";
       			wsi_buttons = lws_client_connect_via_info(&i);
       			while (!wsi_buttons) {
			        wsi_buttons = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
       			}


			/*vTaskDelay(1000/portTICK_PERIOD_MS);
			i.pwsi = &wsi_climate;
			i.protocol = "climate-protocol";
			i.path = "/climate";
       			wsi_climate = lws_client_connect_via_info(&i);
		        while (!wsi_climate) {
			        wsi_climate = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }


			vTaskDelay(1000/portTICK_PERIOD_MS);
			i.pwsi = &wsi_LED;
			i.protocol = "LED-protocol";
			i.path = "/LED";
		        wsi_LED = lws_client_connect_via_info(&i);
		        while (!wsi_LED) {
			        wsi_LED = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }*/
	






			/*i.protocol = "motion-protocol";
			i.path = "/motion";
		        wsi_motion = lws_client_connect_via_info(&i);
		        while (!wsi_motion) {
			        wsi_motion = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }*/

			/*i.pwsi = &wsi_power;
			i.protocol = "power-protocol";
			i.path = "/power";
       			wsi_power = lws_client_connect_via_info(&i);
       			while (!wsi_power) {
			        wsi_power = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(2000/portTICK_PERIOD_MS);
       			}*/
	
			/*i.protocol = "audio-protocol";
			i.path = "/audio";
		        wsi = lws_client_connect_via_info(&i);
		        while (!wsi) {
			        wsi = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }

			i.protocol = "ota-protocol";
			i.path = "/update";
		        wsi = lws_client_connect_via_info(&i);
		        while (!wsi) {
			        wsi = lws_client_connect_via_info(&i);
				taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }

			i.protocol = "esplws-scan";
		        wsi = lws_client_connect_via_info(&i);
		        while (!wsi) {
		        wsi = lws_client_connect_via_info(&i);
			taskYIELD();
				vTaskDelay(1000/portTICK_PERIOD_MS);
		        }*/
			



			while (!lws_service(context, 3000)) {
				taskYIELD();
			}
		}
		// ----------------- //
		// service protocols //
		// ----------------- //
		lws_service(context, 10 * 1000);
		taskYIELD();
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}



void app_main(void)
{

	nvs_flash_init();
	//i2c_example_master_init();
	//gpio_init();
	lws_esp32_wlan_config();
	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL));
	lws_esp32_wlan_start_station();

	memset(&info, 0, sizeof(info));
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols_station;
	info.gid = -1;
	info.uid = -1;
	info.ws_ping_pong_interval = 10;
	context = lws_esp32_init(&info, &vh);

	memset(&i, 0, sizeof i);
	i.address = "192.168.0.10";
        i.port = 4000;
	i.ssl_connection = 0;
	i.host = i.address;
	i.origin = i.host;
        i.ietf_version_or_minus_one = -1;
	i.context = context;

	strcpy(device_type_str,",\"device_type\":[\"");
	strcat(device_type_str,device_type);
	strcat(device_type_str,"\"");

	init_protocols();
	printf("EXITED?!\n");
        return 0;
}
