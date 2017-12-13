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

#define DATA_LENGTH          256  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH       129  /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define CLIMATE_DELAY_TIME   1234 /*!< delay time between different test items */

#define I2C_EXAMPLE_SLAVE_SCL_IO     26    /*!<gpio number for i2c slave clock  */
#define I2C_EXAMPLE_SLAVE_SDA_IO     25    /*!<gpio number for i2c slave data */
#define I2C_EXAMPLE_SLAVE_NUM I2C_NUM_0    /*!<I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN  (2*DATA_LENGTH) /*!<I2C slave tx buffer size */
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN  (2*DATA_LENGTH) /*!<I2C slave rx buffer size */

#define I2C_EXAMPLE_MASTER_SCL_IO    21    /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO    19    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency */


#define TSL4531_SENSOR_ADDR  0x29    /*!< slave address for TSL4531 sensor */
#define TSL4531_CMD   0x04    /*!< Command to set measure mode */

#define SI7020_SENSOR_ADDR          0x40    /*!< slave address for SI7020 sensor */
#define SI7020_CMD_MEASURE_TEMP     0xF3    /*!< Command to set measure mode */
#define SI7020_CMD_MEASURE_HUM      0xF5    /*!< Command to set measure mode */

#define TSL4531_SENSOR_ADDR         0x29    /*!< slave address for SI7020 sensor */
#define TSL4531_CMD_MEASURE_LIGHT   0x04    /*!< Command to set measure mode */


#define BH1750_SENSOR_ADDR  0x29    /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START    0x00    /*!< Command to set measure mode */
#define ESP_SLAVE_ADDR 0x29         /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */


struct lws *wsi_token, *wsi_climate, *wsi_power;
bool power_connected = false;
bool climate_connected = false;
bool token_connected = false;

#include "../components/libwebsockets/plugins/protocol_lws_mirror.c"
#include "../components/libwebsockets/plugins/protocol_post_demo.c"
#include "../components/libwebsockets/plugins/protocol_lws_status.c"
#include "../components/libwebsockets/plugins/protocol_lws_meta.c"
#include <protocol_esp32_lws_reboot_to_factory.c>

#include "plugins/protocol_token.c"
#include "plugins/protocol_climate.c"
#include "plugins/protocol_power.c"

static const struct lws_protocols protocols_station[] = {
	{
		"http-only",
		lws_callback_http_dummy,
		0,
		1024, 0, NULL, 900
	},
	LWS_PLUGIN_PROTOCOL_TOKEN, /* demo... */
	LWS_PLUGIN_PROTOCOL_CLIMATE, /* demo... */
	LWS_PLUGIN_PROTOCOL_POWER, /* demo... */
	LWS_PLUGIN_PROTOCOL_MIRROR,	    /* replace with */
	LWS_PLUGIN_PROTOCOL_POST_DEMO,	    /* your own */
	LWS_PLUGIN_PROTOCOL_LWS_STATUS,	    /* plugin protocol */
	LWS_PLUGIN_PROTOCOL_ESPLWS_RTF,	/* helper protocol to allow reset to factory */
	LWS_PLUGIN_PROTOCOL_LWS_META,	    /* protocol multiplexer */
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
	    wsi_token = NULL;
	    wsi_power = NULL;
	    wsi_climate = NULL;
            //TEST_ESP_OK(esp_wifi_connect());
            break;

        default:
 	    printf("ev_handle_called.\n");
            break;
    }
    //return ESP_OK;

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

static int ratelimit_connects(unsigned int *last, unsigned int secs)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	if (tv.tv_sec - (*last) < secs)
		return 0;

	*last = tv.tv_sec;

	return 1;
}


void app_main(void)
{
	char tag[50] = "[connection-loop]";
	static struct lws_context_creation_info info;
	struct lws_context *context;
	struct lws_vhost *vh;

	lws_esp32_set_creation_defaults(&info);

	info.port = 443;
	info.fd_limit_per_thread = 10;
	info.max_http_header_pool = 12;
	info.max_http_header_data = 512;
	info.pt_serv_buf_size = 900;
	info.keepalive_timeout = 5;
	info.simultaneous_ssl_restriction = 4;
	info.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS |
		       LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

	info.ssl_cert_filepath = "ap-cert.pem";
	info.ssl_private_key_filepath = "ap-key.pem";

	info.vhost_name = "station";
	info.protocols = protocols_station;
	info.mounts = &mount_station;
	info.headers = &pvo_headers;

	nvs_flash_init();
	lws_esp32_wlan_config();

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL));

	lws_esp32_wlan_start_station();
	context = lws_esp32_init(&info, &vh);

 	print_mux = xSemaphoreCreateMutex();
        i2c_example_slave_init();
        i2c_example_master_init();

	gpio_init();
        xTaskCreate(climate_task, "climate_task_0", 1024 * 2, (void* ) 0, 10, NULL);
	xTaskCreate(power_task, "power_task", 1024 * 2, (void* ) 0, 10, NULL);

	static struct lws_client_connect_info i;
	memset(&i, 0, sizeof i);
	i.address = "192.168.0.10";
        i.port = 4000;
	i.ssl_connection = 0;
	i.host = i.address;
	i.origin = i.host;
        i.ietf_version_or_minus_one = -1;
	i.context = context;

	unsigned int rl_token = 0, rl_climate = 0, rl_power = 0, do_ws = 1, pp_secs = 0,
		     do_multi = 0, rl_token_linked = 0, rl_power_linked = 0, rl_climate_linked = 0;

	while (1) {
		
		if (!got_ip) {
	    		vTaskDelay(1000 / portTICK_RATE_MS);
			continue;
		}

		if (!token_connected && ratelimit_connects(&rl_token_linked, 5u)) {
  			printf("[lws_service loop] wsi_token timed out, reconnect\n");
			wsi_token = NULL;
		}
		else
  			printf("[lws_service loop] wsi_token\n");

		if (!climate_connected && ratelimit_connects(&rl_climate_linked, 5u)) {
  			printf("[lws_service loop] wsi_climate timed out, reconnect\n");
			wsi_climate = NULL;
		}

		if (!power_connected && ratelimit_connects(&rl_power_linked, 5u)) {
  			printf("[lws_service loop] wsi_power timed out, reconnect\n");
			wsi_power = NULL;
		}

		if (!wsi_token && ratelimit_connects(&rl_token, 2u)) {
	    		//vTaskDelay(1000 / portTICK_RATE_MS);
			lwsl_notice("token: connecting\n");
			i.protocol = "token-protocol";
			i.pwsi = &wsi_token;
			i.path = "/tokens";
			wsi_token = lws_client_connect_via_info(&i);
		}

		if (!wsi_climate && ratelimit_connects(&rl_climate, 2u)) {
	    		//vTaskDelay(1000 / portTICK_RATE_MS);
			lwsl_notice("climate: connecting\n");
			i.protocol = "climate-protocol";
			i.pwsi = &wsi_climate;
			i.path = "/climate";
			wsi_climate = lws_client_connect_via_info(&i);
		}

		if (!wsi_power && ratelimit_connects(&rl_power, 2u)) {
	    		//vTaskDelay(1000 / portTICK_RATE_MS);
			lwsl_notice("power: connecting\n");
			i.protocol = "power-protocol";
			i.pwsi = &wsi_power;
			i.path = "/power";
			wsi_power = lws_client_connect_via_info(&i);
		}

		lws_service(context, 3000);
		taskYIELD();
 		//vTaskDelay(1000 / portTICK_RATE_MS);

	}
  	printf("EXCITED?!\n");
}
