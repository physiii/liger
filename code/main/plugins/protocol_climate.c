// ---------------------------  OPEN-AUTOMATION -------------------------------- //
// --------------  https://github.com/physiii/open-automation  ----------------- //
// --------------------------- protocol_climate.c ------------------------------ //

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

#include "driver/i2c.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "esp_log.h"

char temp_str[50];
bool climate_received = false;
uint8_t mac[6];
char climate[256];
char previous_climate[256];
char climate_rx_data[256];
char climate_command[100];

char temperature_str[100] = "0";
char humidity_str[100] = "0";
char light_level_str[100] = "0";
bool climate_data_ready = false;

char climate_command[100];
char front_climate_str[100];
char i_str[10];
bool climate_linked = false;
bool climate_req_sent = false;
char climate_str[250] = "";
static bool s_pad_activated[TOUCH_PAD_MAX];
static bool s_pad_activated_notify[TOUCH_PAD_MAX];
static bool s_pad_activated_climate_en[TOUCH_PAD_MAX];
char climate_req_str[1024];

struct per_vhost_data__climate {
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

struct per_session_data__climate {
	int number;
	int value;
};

static void
uv_timeout_cb_climate(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__climate *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__climate, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

// ----------- //
// i2c actions //
// ----------- //
#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define DATA_LENGTH          512  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH       127  /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define CLIMATE_DELAY_TIME    5000 /*!< delay time between different test items */

#define I2C_EXAMPLE_MASTER_SCL_IO    19    /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO    18    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ   400000     /*!< I2C master clock frequency */

#define SI7020_SENSOR_ADDR          0x40    /*!< slave address for SI7020 sensor */
#define SI7020_CMD_MEASURE_TEMP     0xF3    /*!< Command to set measure mode */
#define SI7020_CMD_MEASURE_HUM      0xF5    /*!< Command to set measure mode */

#define TSL4531_SENSOR_ADDR         0x29    /*!< slave address for SI7020 sensor */
#define TSL4531_CMD_MEASURE_LIGHT   0x04    /*!< Command to set measure mode */


#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

static esp_err_t SI7020_measure_temp(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SI7020_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SI7020_CMD_MEASURE_TEMP, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(30 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SI7020_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t SI7020_measure_hum(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SI7020_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SI7020_CMD_MEASURE_HUM, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(30 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SI7020_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t TSL4531_measure_light(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL4531_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL4531_CMD_MEASURE_LIGHT, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(30 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL4531_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static void climate_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t sensor_data_h, sensor_data_l;

    while (1) {
        //--------------------------------------------------//
        ret = SI7020_measure_temp( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
            //printf("temperature: %d\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(temperature_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
        } else {
            printf("SI7020_measure_temp No ack, sensor not connected\n");
        }
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        //--------------------------------------------------//
        ret = SI7020_measure_hum( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("humidity: %d\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(humidity_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
        } else {
            //printf("SI7020_measure_hum No ack, sensor not connected\n");
        }
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        //---------------------------------------------------//
        ret = TSL4531_measure_light( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        //(print_mux, portMAX_DELAY);
        //printf("TASK[%d] MEASURE LIGHT LEVEL TSL4531 (0x00)\n", task_idx);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            printf("MEASURE LIGHT TSL4531 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(light_level_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
        } else {
            printf("TSL4531 No ack, sensor not connected...skip...\n");
        }
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);
	

        //---------------------------------------------------//

        climate_data_ready = true;
    }
}

static int
callback_climate(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[climate-protocol]";

	struct per_session_data__climate *pss =
			(struct per_session_data__climate *)user;
	struct per_vhost_data__climate *vhd =
			(struct per_vhost_data__climate *)
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
		printf("%s initialized\n",tag);
		xTaskCreate(climate_task, "climate_task", 2048, NULL, 10, NULL);
		//break;
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__climate));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);
		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_climate, DUMB_PERIOD, DUMB_PERIOD);
		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		if (!vhd)
			break;
	//	lwsl_notice("di: LWS_CALLBACK_PROTOCOL_DESTROY: v=%p, ctx=%p\n", vhd, vhd->context);
		uv_timer_stop(&vhd->timeout_watcher);
		uv_close((uv_handle_t *)&vhd->timeout_watcher, NULL);
		break;

	case LWS_CALLBACK_CLOSED:
		climate_linked = false;
		break;


	case LWS_CALLBACK_ESTABLISHED:
		pss->number = 0;
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] temperature_str: %s\n",temperature_str);
		if (!token_received) break;

		if (!climate_linked) {
        	        strcpy(climate_req_str, "{\"mac\":\"");
			strcat(climate_req_str,mac_str);
        	        strcat(climate_req_str, "\",\"device_type\":[\"regulator\"]");
        	        strcat(climate_req_str, ",\"cmd\":\"link\"");
        	        strcat(climate_req_str, ",\"token\":\"");
        	        strcat(climate_req_str, token);
        	        strcat(climate_req_str, "\"");
			strcat(climate_req_str,"}");
			n = lws_snprintf((char *)p, sizeof(climate_req_str) - LWS_PRE, "%s", climate_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,climate_req_str);
			}
			break;
		}
		if (!climate_data_ready) break;
		climate_data_ready = false;
                strcpy(climate_req_str, "{\"mac\":\"");
		strcat(climate_req_str,mac_str);
                strcat(climate_req_str, "\",\"temperature\":");
                strcat(climate_req_str, temperature_str);
                strcat(climate_req_str, ",\"humidity\":");
                strcat(climate_req_str, humidity_str);
                strcat(climate_req_str, ",\"light\":");
                strcat(climate_req_str, light_level_str);
                strcat(climate_req_str, ",\"device_type\":[\"regulator\"]");
                strcat(climate_req_str, ",\"cmd\":\"climate\"");
                strcat(climate_req_str, ",\"token\":\"");
                strcat(climate_req_str, token);
                strcat(climate_req_str, "\"");
		strcat(climate_req_str,"}");
		n = lws_snprintf((char *)p, sizeof(climate_req_str) - LWS_PRE, "%s", climate_req_str);
		//printf("%s %s\n",tag,p);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		break;
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			//printf("%s %s\n",tag,climate_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;

		sprintf(climate_command,"%s",(const char *)in);
		//printf("%s %s\n", tag, climate_command);
		if (strcmp(climate_command,"link")) {
			//printf("%s LINKED!!\n", tag);
			climate_linked = true;
		}

		break;

	default:
	   	printf("%s callback_climate: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_CLIMATE \
	{ \
		"climate-protocol", \
		callback_climate, \
		sizeof(struct per_session_data__climate), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_climate
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_climate(struct lws_context *context,
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
destroy_protocol_climate(struct lws_context *context)
{
	return 0;
}

#endif
