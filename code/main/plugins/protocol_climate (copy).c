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
#include "driver/i2c.h"

char climate_command[100];
char tempature_str[100] = "";
char humidity_str[100] = "";
int climate_sum = 0;
bool climate_linked = false;

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

#define CLIMATE_DELAY_TIME   5000 /*!< delay time between different test items */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */






#define SI7020_SENSOR_ADDR          0x40    /*!< slave address for SI7020 sensor */
#define SI7020_CMD_MEASURE_TEMP     0xF3    /*!< Command to set measure mode */
#define SI7020_CMD_MEASURE_HUM     0xF5    /*!< Command to set measure mode */

#define OPT3001_SENSOR_ADDR         0x44    /*!< slave address for SI7020 sensor */
#define OPT3001_CMD_MEASURE_LIGHT   0x00    /*!< Command to set measure mode */

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

static esp_err_t OPT3001_measure_light(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, OPT3001_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, OPT3001_CMD_MEASURE_LIGHT, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(1000 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, OPT3001_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
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
xSemaphoreHandle print_mux;
static void climate_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t sensor_data_h, sensor_data_l;

    while (1) {
        /*--------------------------------------------------*/
        ret = SI7020_measure_temp( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //printf("*******************\n");

        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            sprintf(tempature_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
	    data_ready = true;
        } else {
            printf("SI7020 No ack, sensor not connected...skip...\n");
        }

        xSemaphoreGive(print_mux);
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        /*--------------------------------------------------*/
        ret = SI7020_measure_hum( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //printf("*******************\n");

        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            sprintf(humidity_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
	    data_ready = true;
        } else {
            printf("SI7020 No ack, sensor not connected...skip...\n");
        }

        xSemaphoreGive(print_mux);
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        /*---------------------------------------------------*/
        ret = OPT3001_measure_light( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //printf("*******************\n");
        //printf("TASK[%d] MEASURE LIGHT LEVEL OPT3001 (0x00)\n", task_idx);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            printf("MEASURE LIGHT OPT3001 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
        } else {
            printf("OPT3001 No ack, sensor not connected...skip...\n");
        }

        //xSemaphoreGive(print_mux);
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);
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
	char climate_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		//xTaskCreate(climate_task, "climate_task", 1024*3, &vhd, 10, NULL);
                //xTaskCreate(climate_task, "i2c_test_task_0", 1024 * 2, (void* ) 0, 10, NULL);
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

	case LWS_CALLBACK_ESTABLISHED:
		pss->number = 0;
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] climate_sum: %d\n",climate_sum);
		if (!token_received) break;
		if (!climate_linked) {
        	        strcpy(climate_value_str, "{\"mac\":\"");
			strcat(climate_value_str,mac_str);
        	        strcat(climate_value_str, "\",\"device_type\":[\"regulator\"]");
        	        strcat(climate_value_str, ",\"cmd\":\"link\"");
        	        strcat(climate_value_str, ",\"token\":\"");
        	        strcat(climate_value_str, token);
        	        strcat(climate_value_str, "\"");
			strcat(climate_value_str,"}");
			n = lws_snprintf((char *)p, sizeof(climate_value_str) - LWS_PRE, "%s", climate_value_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n) 
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,climate_value_str);
			}
			break;
		}

		break;

		snprintf(temp_str, 10, "%d",climate_sum);
                strcpy(climate_value_str, "{\"mac\":\"");
		strcat(climate_value_str,mac_str);
                strcat(climate_value_str, "\",\"value\":");
                strcat(climate_value_str, temp_str);
                strcat(climate_value_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(climate_value_str, ",\"cmd\":\"climate\"");
                strcat(climate_value_str, ",\"token\":\"");
                strcat(climate_value_str, token);
                strcat(climate_value_str, "\"");
		strcat(climate_value_str,"}");

		n = lws_snprintf((char *)p, sizeof(climate_value_str) - LWS_PRE, "%s", climate_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",tag,climate_value_str);
		} else
			printf("%s %s\n",tag,climate_value_str);
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;

		sprintf(climate_command,"%s",(const char *)in);
		printf("%s %s\n", tag, climate_command);
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
