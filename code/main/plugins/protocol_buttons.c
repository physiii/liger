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
#include "esp_system.h"
#include "nvs_flash.h"

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define BUTTON1 (4)
#define BUTTON2 (5)
#define BUTTON3 (6)
#define BUTTON4 (7)
#define LIGHT_SWITCH    15
#define GPIO_OUTPUT_IO_1    19
#define GPIO_OUTPUT_PIN_SEL  ((1<<LIGHT_SWITCH) | (1<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1<<GPIO_INPUT_IO_0) | (1<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

uint8_t mac[6];
char mac_str[20];
int value[SAMPLE_SIZE];
int button_value;
char button1_str[50];
char button2_str[50];
char button3_str[50];
char button4_str[50];
int button1_sum = 0;
int button2_sum = 0;
int button3_sum = 0;
int button4_sum = 0;
bool light_state = false;

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

//extern int value[1024];

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

void adc2task(struct per_vhost_data__buttons *vhd)
{
	char tag[50] = "[buttons-protocol]";
	bool button_pressed = false;
	while(1){
		button1_sum = 0;
		button2_sum = 0;
		button3_sum = 0;
		button4_sum = 0;
		for (int i = 0; i < SAMPLE_SIZE; i++) {
			button1_sum+=adc1_get_voltage(BUTTON1);
			button2_sum+=adc1_get_voltage(BUTTON2);
			button3_sum+=adc1_get_voltage(BUTTON3);
			button4_sum+=adc1_get_voltage(BUTTON4);
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
		if (button1_sum > 100000 || button2_sum > 100000 || button3_sum > 100000 || button4_sum > 100000) {
			if (button_pressed) continue;
			button_pressed = true;
			printf("%s Button pressed: %s\n", tag, button_pressed ? "true" : "false");
		        gpio_set_level(LIGHT_SWITCH, button_pressed);
		} else {
			if (!button_pressed) continue;
			button_pressed = false;
			printf("%s Button released: %s\n", tag, button_pressed ? "true" : "false");
		        gpio_set_level(LIGHT_SWITCH, button_pressed);
		}
	}
}

// --------- gpio --------- //
static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void init_gpio()
{
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

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    int cnt = 0;
    while(1) {
        //printf("cnt: %d\n", cnt++);
        vTaskDelay(1000 / portTICK_RATE_MS);
        //gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
        //gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
    }
}

// ------------------------ //

static int
callback_buttons(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[buttons-protocol]";
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
	char button_value_str[1024];
	int n, m;
	switch (reason) {
	case 1: //conn err
		lwsl_notice("\nthe request client connection has been unable to complete a handshake with the remote server.\n");
		break;

	case LWS_CALLBACK_PROTOCOL_INIT:
		printf("%s initialize\n",tag);
		xTaskCreate(adc2task, "adc2task", 1024*3, &vhd, 10, NULL);
		xTaskCreate(init_gpio, "init_gpio", 1024*3, &vhd, 10, NULL);
		//xTaskCreate(read_sens_task, "read_sens_task", 1024*3, &vhd, 10, NULL);

		// initialize ADC
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_config_channel_atten(BUTTON1,ADC_ATTEN_11db);

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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] button1_sum: %d\n",button1_sum);
		if (button1_sum < 100000 && button2_sum < 100000 && button3_sum < 100000 && button4_sum < 100000)
			break;
		snprintf(button1_str, 10, "%d",button1_sum);
		snprintf(button2_str, 10, "%d",button2_sum);
		snprintf(button3_str, 10, "%d",button3_sum);
		snprintf(button4_str, 10, "%d",button4_sum);
                strcpy(button_value_str, "{");
	                strcat(button_value_str, "\"mac\":\"");
			strcat(button_value_str,mac_str);
                strcat(button_value_str, "\",");
	                strcat(button_value_str, "\"button1\":");
			strcat(button_value_str,button1_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button2\":");
			strcat(button_value_str,button2_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button3\":");
			strcat(button_value_str,button3_str);
                strcat(button_value_str, ",");
        	        strcat(button_value_str, "\"button4\":");
			strcat(button_value_str,button4_str);
                strcat(button_value_str, ",");
	       	        strcat(button_value_str, "\"token\":\"");
			strcat(button_value_str,token);
		strcat(button_value_str,"\"}");
		n = lws_snprintf((char *)p, sizeof(button_value_str) - LWS_PRE, "%s", button_value_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			printf("%s %s\n",tag,button_value_str);
		} else {
			//printf("%s %s\n",tag,button_value_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 6)
			break;
		printf("%s[LWS_CALLBACK_CLIENT_RECEIVE] %s\n",tag,(const char *)in);
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
	   	printf("%s callback_buttons: %d\n",tag,reason);
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
