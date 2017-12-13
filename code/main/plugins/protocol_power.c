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

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

char temp_str[50];
bool power_received = false;
uint8_t mac[6];
char power_rx_data[256];
char power_command[100];

char current_str[100] = "0";
char voltage_str[100] = "0";

bool power_data_ready = false;

char power_command[100];
char front_power_str[100];
char i_str[10];
int power_linked = 0;
//bool power_connected = false;

char power_str[250] = "";
static bool s_pad_activated[TOUCH_PAD_MAX];
static bool s_pad_activated_notify[TOUCH_PAD_MAX];
static bool s_pad_activated_USB_POWER_PIN[TOUCH_PAD_MAX];
char power_req_str[1024];

uint32_t low_battery_off = 0 * 1000;
uint32_t low_battery_on = 0 * 1000;
uint32_t high_battery_off = 20 * 1000;
uint32_t high_battery_on =  20 * 1000;
uint32_t battery_power = 0;

uint32_t current_cf = 1000;
uint32_t current_cb = 0;
uint32_t voltage_cf =  1000;
uint32_t voltage_cb = 0;
uint32_t power_cf =  1000;
uint32_t power_cb = 0;


char temp_str[50];
char mac_str[20];
char power[512];
char power_message[512];
char previous_power[256];
char battery_power_str[50];
char usb_state[10];


bool low_battery = false;
bool power_req_sent = false;
bool power_connect = true;
//bool power_connecting = false;
bool INA219_CONFIGURED = false;
uint8_t mac[6];

int battery_voltage = 0;
int battery_current = 0;
int usb_power_value = 0;
int panel_en_value = 0;

struct per_vhost_data__power {
	uv_timer_t timeout_watcher;
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;
};

struct per_session_data__power {
	int number;
};

static void
uv_timeout_cb_power(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__power *vhd = lws_container_of(w,
			struct per_vhost_data__power, timeout_watcher);
	lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}


#define POWER_DELAY_TIME   1234 /*!< delay time between different test items */

#define INA219_SENSOR_ADDR            0x41    /*!< slave address for SI7020 sensor */
#define INA219_CMD_CONFIGURE          0x00    /*!< Command to set measure mode */
#define INA219_CONFIGURATION_MSB      0x11    /*!< Command to set measure mode */
#define INA219_CONFIGURATION_LSB      0x9F    /*!< Command to set measure mode */
#define INA219_CMD_MEASURE_CURRENT    0x01    /*!< Command to set measure mode */
#define INA219_CMD_MEASURE_VOLTAGE    0x02    /*!< Command to set measure mode */
#define INA219_CMD_MEASURE_POWER      0x03    /*!< Command to set measure mode */

#define USB_POWER_PIN 17
#define PANEL_EN 16
#define GPIO_OUTPUT_PIN_SEL  ((1<<USB_POWER_PIN) | (1<<PANEL_EN))
#define ESP_INTR_FLAG_DEFAULT 0

xSemaphoreHandle print_mux;

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


void gpio_init() {
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(USB_POWER_PIN, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(USB_POWER_PIN, gpio_isr_handler, (void*) USB_POWER_PIN);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PANEL_EN, gpio_isr_handler, (void*) PANEL_EN);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(USB_POWER_PIN);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(USB_POWER_PIN, gpio_isr_handler, (void*) USB_POWER_PIN);

    //printf("turn on PANEL_EN\n");
    //gpio_set_level(PANEL_EN, 1);
}

static esp_err_t INA219_configure(i2c_port_t i2c_num, uint8_t* data_msb, uint8_t* data_lsb, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, ( INA219_SENSOR_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CMD_CONFIGURE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CONFIGURATION_MSB, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CONFIGURATION_LSB, ACK_CHECK_EN);
    /*i2c_master_write(cmd, data_msb, size, ACK_CHECK_EN);
    i2c_master_write(cmd, data_lsb, size, ACK_CHECK_EN);*/

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ESP_OK;
}

static esp_err_t INA219_measure_current(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CMD_MEASURE_CURRENT, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(30 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
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

static esp_err_t INA219_measure_voltage(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CMD_MEASURE_VOLTAGE, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(30 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
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

static esp_err_t INA219_measure_power(i2c_port_t i2c_num, uint8_t* data_h, uint8_t* data_l)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, INA219_CMD_MEASURE_POWER, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }

    vTaskDelay(100 / portTICK_RATE_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, INA219_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
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

static void power_task(void* arg)
{
    char tag[20] = "[climate-protocol]";
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;

    uint8_t* data_msb = (uint8_t*) INA219_CONFIGURATION_MSB;
    uint8_t* data_lsb = (uint8_t*) INA219_CONFIGURATION_LSB;
    
    low_battery_on = get_u32("low_battery_on",low_battery_on);
    low_battery_off = get_u32("low_battery_off",low_battery_off);
    high_battery_on = get_u32("high_battery_on",high_battery_on);
    high_battery_off = get_u32("high_battery_off",high_battery_off);

    voltage_cf = get_u32("voltage_cf",voltage_cf);
    voltage_cb = get_u32("voltage_cb",voltage_cb);
    current_cf = get_u32("current_cf",current_cf);
    current_cb = get_u32("current_cb",current_cb);

    while (1) {

        //--------------------------------------------------//
	if (!INA219_CONFIGURED) {
		ret = INA219_configure( I2C_EXAMPLE_MASTER_NUM, data_msb, data_lsb, RW_TEST_LENGTH);
        	if (ret == ESP_OK) {
        	    //printf("INA219 configured\n");
		    INA219_CONFIGURED = true;
	        } else {
        	    printf("INA219_CONFIGURED No ack, sensor not connected\n");
	        }
	        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);
	}

        //--------------------------------------------------//
	ret = INA219_measure_current( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            //printf("MEASURE CURRENT INA219 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
	    battery_current = ( sensor_data_h << 8 | sensor_data_l ) * current_cf / 1000 - current_cb;
            sprintf(current_str,"%d", battery_current);
	    //power_data_ready = true;
        } else {
            printf("INA219_measure_current No ack, sensor not connected\n");
        }
        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        //--------------------------------------------------//
        ret = INA219_measure_voltage( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
	   battery_voltage = ( sensor_data_h << 8 | sensor_data_l );
	   battery_voltage = battery_voltage * voltage_cf / 1000 - voltage_cb;
	   //printf("battery voltage %d mV\n",battery_voltage);
           sprintf(voltage_str,"%d", battery_voltage);
           power_data_ready = true;
        } else {
            printf("INA219_measure_voltage No ack, sensor not connected\n");
        }
        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);
        //--------------------------------------------------//
        /*ret = INA219_measure_power( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        if (ret == ESP_OK) {
	   battery_power = ( sensor_data_h << 8 | sensor_data_l );
           sprintf(battery_power_str,"%d", battery_power);
           power_data_ready = true;
        } else {
            printf("INA219_measure_power No ack, sensor not connected\n");
        }
        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);*/
        //--------------------------------------------------//

	if (battery_voltage < low_battery_off && !low_battery) {
		low_battery = true;
		printf("%s low battery (%d), turning off power\n", tag, battery_voltage);
		usb_power_value = 100;
                gpio_set_level(USB_POWER_PIN, usb_power_value);
	}
	if (battery_voltage > low_battery_on && low_battery) {
		low_battery = false;
		printf("%s battery charged (%d), turning on power\n", tag, battery_voltage);
		usb_power_value = 0;
                gpio_set_level(USB_POWER_PIN, usb_power_value);
	}
    }
}

static int
callback_power(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[20] = "[power-protocol]";
	struct per_session_data__power *pss =
			(struct per_session_data__power *)user;
	struct per_vhost_data__power *vhd =
			(struct per_vhost_data__power *)
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
		power_connect = false;
		break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		//power_connect = false;
		//power_linked = false;
		power_connected = true;
		pss->number = 0;
		printf("%s LWS_CALLBACK_CLIENT_ESTABLISHED\n",tag);
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		printf("%s LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n",tag);
		power_linked = false;
		power_req_sent = false;
		wsi_power = NULL;
		break;

	case LWS_CALLBACK_CLOSED:
		printf("%s LWS_CALLBACK_CLOSED\n", tag);
		power_linked = false;
		power_req_sent = false;
		power_connected = false;
		wsi_power = NULL;
		break;

	case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
		//printf("%s LWS_CALLBACK_HTTP_DROP_PROTOCOL\n", tag);
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialized\n",tag);
		//xTaskCreate(power_task, "power_task", 2048, NULL, 10, NULL);

    /*print_mux = xSemaphoreCreateMutex();
    i2c_example_slave_init();
    i2c_example_master_init();*/



		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__power));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_get_protocol(wsi);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_power, DUMB_PERIOD, DUMB_PERIOD);

		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		if (!vhd)
			break;
		lwsl_notice("di: LWS_CALLBACK_PROTOCOL_DESTROY: v=%p, ctx=%p\n", vhd, vhd->context);
		uv_timer_stop(&vhd->timeout_watcher);
		uv_close((uv_handle_t *)&vhd->timeout_watcher, NULL);
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		printf("%s [LWS_CALLBACK_CLIENT_WRITEABLE] %d\n",tag,power_req_sent);
		if (!token_linked) break;
		if (!power_linked) {
			if (power_req_sent) break;
        	        strcpy(power_req_str, "{\"mac\":\"");
			strcat(power_req_str,mac_str);
        	        strcat(power_req_str, "\",\"device_type\":[\"regulator\"]");
        	        strcat(power_req_str, ",\"cmd\":\"link\"");
        	        strcat(power_req_str, ",\"token\":\"");
        	        strcat(power_req_str, token);
        	        strcat(power_req_str, "\"");
			strcat(power_req_str,"}");
			n = lws_snprintf((char *)p, sizeof(power_req_str) - LWS_PRE, "%s", power_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to power socket\n", n);
			else  {
				power_req_sent = true;
				//printf("%s %s\n",tag,power_req_str);
			}
			break;
		}
		if (!power_data_ready) break;
		power_data_ready = false;
		if (usb_power_value) strcpy(usb_state,"OFF");
		else strcpy(usb_state,"ON");

		battery_power = battery_voltage * battery_voltage;
                sprintf(battery_power_str,"%d", battery_power);

                strcpy(power_req_str, "{\"mac\":\"");
		strcat(power_req_str,mac_str);
                strcat(power_req_str, "\",\"voltage\":");
                strcat(power_req_str, voltage_str);
                strcat(power_req_str, ",\"current\":");
                strcat(power_req_str, current_str);
                strcat(power_req_str, ",\"power\":");
                strcat(power_req_str, battery_power_str);
                strcat(power_req_str, ",\"usb_state\":\"");
                strcat(power_req_str, usb_state);
                strcat(power_req_str, "\",\"device_type\":[\"regulator\"]");
                strcat(power_req_str, ",\"cmd\":\"power\"");
                strcat(power_req_str, ",\"token\":\"");
                strcat(power_req_str, token);
                strcat(power_req_str, "\"");
		strcat(power_req_str,"}");
		n = lws_snprintf((char *)p, sizeof(power_req_str) - LWS_PRE, "%s", power_req_str);
		//printf("%s %s\n",tag,p);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		break;
		if (m < n) {
			lwsl_err("error %d writing to power socket\n", n);
		}
		else  {
			//printf("%s %s\n",tag,power_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 1)
			break;
		//sprintf(power_command,"%s",(const char *)in);
		strcat(power_command,(const char *)in);
		//printf("%s power_command: %s\n",tag, power_command);
		//printf("token_message: %s\n",token_message);
		int open_bra = char_count('{',power_command);
		int close_bra = char_count('}',power_command);
		
		if (close_bra == 0) break;
		if (close_bra != open_bra) break;
		if (power_command[0] != '{') break;
		if (power_command[strlen(power_command) - 1] != '}') break;
		//printf("%s power_command: %s\n",tag, power_command);

		cJSON *root = cJSON_Parse(power_command);
	
		if (cJSON_GetObjectItem(root,"linked"))
			power_linked = cJSON_GetObjectItem(root,"linked")->valueint;
		
		if (cJSON_GetObjectItem(root, "settings")) {
			cJSON *new_settings = cJSON_GetObjectItem(root, "settings");

		if (cJSON_GetObjectItem(new_settings,"set_low_battery")) {
			cJSON *set_low_battery = cJSON_GetObjectItem(new_settings,"set_low_battery");
			low_battery_on = cJSON_GetObjectItem(set_low_battery,"on")->valueint;
			low_battery_off = cJSON_GetObjectItem(set_low_battery,"off")->valueint;
			store_u32("low_battery_on",low_battery_on);
			store_u32("low_battery_off",low_battery_off);
		}
	
		if (cJSON_GetObjectItem(new_settings,"set_high_battery")) {
			cJSON *set_high_battery = cJSON_GetObjectItem(new_settings,"set_high_battery");
			high_battery_on = cJSON_GetObjectItem(set_high_battery,"on")->valueint;
			high_battery_off = cJSON_GetObjectItem(set_high_battery,"off")->valueint;
			store_u32("high_battery_on",high_battery_on);
			store_u32("high_battery_off",high_battery_off);
		}

		if (cJSON_GetObjectItem(new_settings,"set_voltage")) {
			cJSON *set_voltage = cJSON_GetObjectItem(new_settings,"set_voltage");
			voltage_cf = cJSON_GetObjectItem(set_voltage,"factor")->valueint;
			voltage_cb = cJSON_GetObjectItem(set_voltage,"bias")->valueint;
			store_u32("voltage_cf",voltage_cf);
			store_u32("voltage_cb",voltage_cb);
		}

		if (cJSON_GetObjectItem(new_settings,"set_current")) {
			cJSON *set_current = cJSON_GetObjectItem(new_settings,"set_current");
			current_cf = cJSON_GetObjectItem(set_current,"factor")->valueint;
			current_cb = cJSON_GetObjectItem(set_current,"bias")->valueint;
			store_u32("current_cf",current_cf);
			store_u32("current_cb",current_cb);
		}

		if (cJSON_GetObjectItem(new_settings,"set_usb_power")) {
			usb_power_value = cJSON_GetObjectItem(new_settings,"set_usb_power")->valueint;
			store_u32("usb_power",usb_power_value);
			gpio_set_level(USB_POWER_PIN, usb_power_value);
		}
	
		if (cJSON_GetObjectItem(new_settings,"command")) {
			char * command = cJSON_GetObjectItem(new_settings,"command")->valuestring;
			if (!strcmp(command,"power_off")) {
				printf("%s turning power off!\n", tag);
	               	        //printf("%s setting USB_POWER_PIN pin to %d to %d\n", tag, USB_POWER_PIN, USB_POWER_PINvalue);	
				usb_power_value = 100;
	               	        gpio_set_level(USB_POWER_PIN, usb_power_value);
				//USB_POWER_PIN(USB_POWER_PIN,usb_power_value);
			}
	
			if (!strcmp(command,"power_on")) {
				printf("%s turning power on!\n", tag);
				usb_power_value = 0;
	               	        //printf("%s setting USB_POWER_PIN pin to %d to %d\n", tag, USB_POWER_PIN, USB_POWER_PINvalue);	
	               	        gpio_set_level(USB_POWER_PIN, usb_power_value);
				//USB_POWER_PIN(USB_POWER_PIN,usb_power_value);
			}
		}

		}

		strcpy(power_command,"");
		if (strlen(power_command) > sizeof(power_command) - 1) strcpy(power_command,"");
		break;



	default:
	   	printf("%s callback_power: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_POWER \
	{ \
		"power-protocol", \
		callback_power, \
		sizeof(struct per_session_data__power), \
		10, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_POWER
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_power(struct lws_context *context,
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
destroy_protocol_power(struct lws_context *context)
{
	return 0;
}

#endif
