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
#include "driver/i2c.h"
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"




char temp_str[50];
bool climate_received = false;
uint8_t mac[6];
char climate_rx_data[256];
char climate_command[256];

char temperature_str[100] = "0";
char humidity_str[100] = "0";
char light_level_str[100] = "0";
bool climate_data_ready = false;

char front_climate_str[100];
char i_str[10];
int climate_linked = 0;
//bool climate_connected = false;

char climate_str[250] = "";
static bool s_pad_activated[TOUCH_PAD_MAX];
static bool s_pad_activated_notify[TOUCH_PAD_MAX];
static bool s_pad_activated_climate_en[TOUCH_PAD_MAX];
char climate_req_str[1024];

uint32_t tempature_cf = 1000;
uint32_t tempature_cb = 0;
uint32_t humidity_cf =  1000;
uint32_t humidity_cb = 0;
uint32_t light_cf =  1000;
uint32_t light_cb = 0;


char temp_str[50];
char mac_str[20];
char climate[512];
char climate_message[512];
char previous_climate[256];

bool climate_req_sent = false;
bool climate_connect = true;
//bool climate_connecting = false;
uint8_t mac[6];



struct per_vhost_data__climate {
	uv_timer_t timeout_watcher;
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;
};

struct per_session_data__climate {
	int number;
};

static void
uv_timeout_cb_climate(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__climate *vhd = lws_container_of(w,
			struct per_vhost_data__climate, timeout_watcher);
	lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}



xSemaphoreHandle print_mux;

static esp_err_t power_on_TSL4531(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x29 << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x00  | 0x80, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x03, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }
    vTaskDelay(100 / portTICK_RATE_MS);
    return ESP_OK;
}

static esp_err_t read_high_TSL4531(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x29 << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x04 | 0x80, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }
    vTaskDelay(100 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x29 << 1 | READ_BIT, ACK_CHECK_EN);
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

/**
 * @brief i2c master initialization
 */
static void i2c_example_master_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief i2c slave initialization
 */
static void i2c_example_slave_init()
{
    int i2c_slave_port = I2C_EXAMPLE_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_EXAMPLE_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_EXAMPLE_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config(i2c_slave_port, &conf_slave);
    i2c_driver_install(i2c_slave_port, conf_slave.mode,
                       I2C_EXAMPLE_SLAVE_RX_BUF_LEN,
                       I2C_EXAMPLE_SLAVE_TX_BUF_LEN, 0);
}

/**
 * @brief test function to show buffer
 */
static void disp_buf(uint8_t* buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if (( i + 1 ) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

static void climate_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;

        ret = power_on_TSL4531( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_OK) {
            //printf("TSL4531 POWERED ON\n");
        } else {
            printf("TSL4531 No ack, sensor not connected...skip...\n");
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);


    while (1) {



        //--------------------------------------------------//
        ret = SI7020_measure_temp( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
            //printf("temperature: %d\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(temperature_str,"%d", ( sensor_data_h << 8 | sensor_data_l ) * tempature_cf / 1000 + tempature_cb);
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
            sprintf(humidity_str,"%d", ( sensor_data_h << 8 | sensor_data_l ) * humidity_cf / 1000 + humidity_cb);
        } else {
            //printf("SI7020_measure_hum No ack, sensor not connected\n");
        }
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        //---------------------------------------------------//

        ret = read_high_TSL4531( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            sprintf(light_level_str,"%d", ( sensor_data_h << 8 | sensor_data_l ) * light_cf / 1000 + light_cb);
        } else {
            printf("TSL4531 No ack, sensor not connected...skip...\n");
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        climate_data_ready = true;

        /*ret = TSL4531_measure_light( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        //(print_mux, portMAX_DELAY);
        //printf("TASK[%d] MEASURE LIGHT LEVEL TSL4531 (0x00)\n", task_idx);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            //printf("MEASURE LIGHT TSL4531 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(light_level_str,"%d", ( sensor_data_h << 8 | sensor_data_l ) * light_cf / 1000 + light_cb);
        } else {
            printf("TSL4531 No ack, sensor not connected...skip...\n");
        }
        vTaskDelay(( CLIMATE_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);*/

        /*for (i = 0; i < DATA_LENGTH; i++) {
            data[i] = i;
        }
        size_t d_size = i2c_slave_write_buffer(I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        if (d_size == 0) {
            printf("i2c slave tx buffer full\n");
            ret = i2c_example_master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, DATA_LENGTH);
        } else {
            ret = i2c_example_master_read_slave(I2C_EXAMPLE_MASTER_NUM, data_rd, RW_TEST_LENGTH);
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        printf("*******************\n");
        printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
        printf("*******************\n");
        printf("====TASK[%d] Slave buffer data ====\n", task_idx);
        disp_buf(data, d_size);
        if (ret == ESP_OK) {
            printf("====TASK[%d] Master read ====\n", task_idx);
            disp_buf(data_rd, d_size);
        } else {
            printf("Master read slave error, IO not connected...\n");
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);*/
        //---------------------------------------------------
        /*int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        //we need to fill the slave buffer so that master can read later
        ret = i2c_example_master_write_slave( I2C_EXAMPLE_MASTER_NUM, data_wr, RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            size = i2c_slave_read_buffer( I2C_EXAMPLE_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        printf("*******************\n");
        printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
        printf("*******************\n");
        printf("----TASK[%d] Master write ----\n", task_idx);
        disp_buf(data_wr, RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            printf("TASK[%d] Master write slave error, IO not connected....\n", task_idx);
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( DELAY_TIME_BETWEEN_ITEMS_MS * ( task_idx + 1 ) ) / portTICK_RATE_MS);*/
    }
}

static int
callback_climate(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[20] = "[climate-protocol]";
	struct per_session_data__climate *pss =
			(struct per_session_data__climate *)user;
	struct per_vhost_data__climate *vhd =
			(struct per_vhost_data__climate *)
			lws_protocol_vh_priv_get(lws_get_vhost(wsi),
					lws_get_protocol(wsi));
	unsigned char buf[LWS_PRE + 1024];
	unsigned char *p = &buf[LWS_PRE];
	int n, m;

        esp_wifi_get_mac(WIFI_IF_STA,mac);
	sprintf(mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
           mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);

	switch (reason) {

	case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
		//printf("%s LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH\n",tag);
		//climate_connect = false;
		break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		//climate_connect = false;
		climate_connected = true;
		pss->number = 0;
		printf("%s LWS_CALLBACK_CLIENT_ESTABLISHED\n",tag);
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		printf("%s LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n",tag);
		climate_linked = false;
		climate_req_sent = false;
		wsi_climate = NULL;
		break;

	case LWS_CALLBACK_CLOSED:
		printf("%s LWS_CALLBACK_CLOSED\n", tag);
		climate_connected = false;
		climate_linked = false;
		climate_req_sent = false;
		wsi_climate = NULL;
		break;

	case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
		//printf("%s LWS_CALLBACK_HTTP_DROP_PROTOCOL\n", tag);
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialized\n",tag);
		//xTaskCreate(climate_task, "climate_task", 2048, NULL, 10, NULL);

		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__climate));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_get_protocol(wsi);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_climate, DUMB_PERIOD, DUMB_PERIOD);

		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		if (!vhd)
			break;
		lwsl_notice("di: LWS_CALLBACK_PROTOCOL_DESTROY: v=%p, ctx=%p\n", vhd, vhd->context);
		uv_timer_stop(&vhd->timeout_watcher);
		uv_close((uv_handle_t *)&vhd->timeout_watcher, NULL);
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		printf("%s [LWS_CALLBACK_CLIENT_WRITEABLE] %d\n",tag,climate_req_sent);
		if (!token_linked) break;
		if (!climate_linked) {
			if (climate_req_sent) break;
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
				lwsl_err("ERROR %d writing to climate socket\n", n);
			else  {
				climate_req_sent = true;
				//printf("%s %s\n",tag,climate_req_str);
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
		if (m < n) {
			lwsl_err("error %d writing to climate socket\n", n);
		}
		else  {
			//printf("%s %s\n",tag,climate_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 1)
			break;
		//sprintf(climate_command,"%s",(const char *)in);
		strcat(climate_command,(const char *)in);
		//printf("%s climate_command: %s\n",tag, climate_command);

		int open_bra = char_count('{',climate_command);
		int close_bra = char_count('}',climate_command);
		
		if (open_bra == close_bra) {
			cJSON *root = cJSON_Parse(climate_command);

			if (cJSON_GetObjectItem(root,"linked"))
				climate_linked = cJSON_GetObjectItem(root,"linked")->valueint;

			if (cJSON_GetObjectItem(root,"set_temperature")) {
				cJSON *set_temperature = cJSON_GetObjectItem(root,"set_temperature");
				tempature_cf = cJSON_GetObjectItem(set_temperature,"new_factor")->valueint;
				tempature_cb = cJSON_GetObjectItem(set_temperature,"new_bias")->valueint;
				store_u32("tempature_cf",tempature_cf);
				store_u32("tempature_cb",tempature_cb);
			}

			if (cJSON_GetObjectItem(root,"set_humidity")) {
				cJSON *set_humidity = cJSON_GetObjectItem(root,"set_humidity");
				humidity_cf = cJSON_GetObjectItem(set_humidity,"new_factor")->valueint;
				humidity_cb = cJSON_GetObjectItem(set_humidity,"new_bias")->valueint;
				store_u32("humidity_cf",humidity_cf);
				store_u32("humidity_cb",humidity_cb);
			}

			if (cJSON_GetObjectItem(root,"set_light")) {
					cJSON *set_light = cJSON_GetObjectItem(root,"set_light");
			light_cf = cJSON_GetObjectItem(set_light,"new_factor")->valueint;
				light_cb = cJSON_GetObjectItem(set_light,"new_bias")->valueint;
				store_u32("light_cf",light_cf);
				store_u32("light_cb",light_cb);
			}
			strcpy(climate_command,"");
		}
		//printf("length of climate_command is %d\n",strlen(climate_command));
		if (strlen(climate_command) > sizeof(climate_command) - 1) strcpy(climate_command,"");
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
		10, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_CLIMATE
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
