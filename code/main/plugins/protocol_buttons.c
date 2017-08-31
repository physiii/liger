// ---------------------------  OPEN-AUTOMATION -------------------------------- //
// --------------  https://github.com/physiii/open-automation  ----------------- //
// --------------------------- protocol_buttons.c ------------------------------ //

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

#include "driver/touch_pad.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "esp_log.h"

#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define SAMPLE_SIZE (128)
#define SAMPLE_RATE (44100)



#define LIGHT1    15
#define LIGHT2    20

#define GPIO_OUTPUT_PIN_SEL  ((1<<LIGHT1) | (1<<LIGHT2))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1<<GPIO_INPUT_IO_0) | (1<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

int light1_value = 0;
int light2_value = 0;
int light3_value = 0;
int light4_value = 0;



char temp_str[50];
bool buttons_received = false;
uint8_t mac[6];
char mac_str[20];
char buttons[256];
char previous_buttons[256];
char buttons_rx_data[256];
char button_command[100];
char front_button_str[100];
char i_str[10];
bool buttons_linked = false;
bool button_req_sent = false;
char button_str[250] = "";
static bool s_pad_activated[TOUCH_PAD_MAX];
static bool s_pad_activated_notify[TOUCH_PAD_MAX];
static bool s_pad_activated_light[TOUCH_PAD_MAX];
bool touch_activated = false;
char buttons_req_str[1024];

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

// ------------- //
// light actions //
// ------------- //
bool light1_hold = false;

static void light_on(int channel, int value) {
	char tag[50] = "[buttons-protocol]";
        printf("%s setting channel: %d to %d\n", tag, channel, value);
        gpio_set_level(LIGHT1, value);
}

static void light_task(void* arg) {
    char tag[50] = "[buttons-protocol]";
    while(1) {

        for (int i=0; i<TOUCH_PAD_MAX; i++) {
            if (s_pad_activated_light[i] == true) {
		if (i == 1) continue;
		s_pad_activated_light[i] = false;
        	//printf("%s light hold: %d \n", tag, light1_hold);
		if (!light1_hold) {
			if (light1_value > 50) {light1_value = 0;}
			else {light1_value = 100;}
			light_on(LIGHT1, light1_value);
			LED_G_value = light1_value * 80;
			//LED_loop();
		        //gpio_set_level(LIGHT1, light1_value);
			light1_hold = true;
	        	vTaskDelay(1000 / portTICK_PERIOD_MS);
			light1_hold = false;
		}
            }
        }
       	vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void light_fade(int velocity, bool direction) {

}


/*
  Read values sensed at all available touch pads.
  Use half of read value as the threshold
  to trigger interrupt when the pad is touched.
  Note: this routine demonstrates a simple way
  to configure activation threshold for the touch pads.
  Do not touch any pads when this routine
  is running (on application start).
 */
static void tp_example_set_thresholds(void)
{
    uint16_t touch_value;
    for (int i=0; i<TOUCH_PAD_MAX; i++) {
        ESP_ERROR_CHECK(touch_pad_read(i, &touch_value));
        printf("T%d intial value: %d\n", i, touch_value);
        ESP_ERROR_CHECK(touch_pad_config(i, touch_value/2));
    }
}

/*
  Check if any of touch pads has been activated
  by reading a table updated by rtc_intr()
  If so, then print it out on a serial monitor.
  Clear related entry in the table afterwards
 */
static void tp_example_read_task(void *pvParameter)
{
    while (1) {
        for (int i=0; i<TOUCH_PAD_MAX; i++) {
            //printf("Checking T%d...\n", i);
            if (s_pad_activated[i] == true) {
                //printf("T%d activated!\n", i);
                s_pad_activated_notify[i] = true;
                s_pad_activated_light[i] = true;
                s_pad_activated[i] = false;
                vTaskDelay(200 / portTICK_PERIOD_MS);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}



/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
static void tp_example_rtc_intr(void * arg)
{
    uint32_t pad_intr = READ_PERI_REG(SENS_SAR_TOUCH_CTRL2_REG) & 0x3ff;
    uint32_t rtc_intr = READ_PERI_REG(RTC_CNTL_INT_ST_REG);
    //clear interrupt
    WRITE_PERI_REG(RTC_CNTL_INT_CLR_REG, rtc_intr);
    SET_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL2_REG, SENS_TOUCH_MEAS_EN_CLR);

    if (rtc_intr & RTC_CNTL_TOUCH_INT_ST) {
        for (int i = 0; i < TOUCH_PAD_MAX; i++) {
            if ((pad_intr >> i) & 0x01) {
                s_pad_activated[i] = true;
            }
        }
    }
}

// ---- //
// gpio //
// ---- //

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static int
callback_buttons(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[buttons-protocol]";

	struct per_session_data__buttons *pss =
			(struct per_session_data__buttons *)user;
	struct per_vhost_data__buttons *vhd =
			(struct per_vhost_data__buttons *)
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

		// -----------//
		// touch init //
		// -----------//
		touch_pad_init();
		tp_example_set_thresholds();
		touch_pad_isr_handler_register(tp_example_rtc_intr, NULL, 0, NULL);
		xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);
	
		// -----------//
		// light init //
		// -----------//
		gpio_config_t io_conf;
		io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
		io_conf.mode = GPIO_MODE_OUTPUT;
		io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
		io_conf.pull_down_en = 0;
		io_conf.pull_up_en = 0;
		gpio_config(&io_conf);
		xTaskCreate(light_task, "light_task", 2048, NULL, 10, NULL);

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
		//printf("[LWS_CALLBACK_CLIENT_WRITEABLE] sum: %d\n",sum);
		if (!token_received) break;
		//if (button_req_sent) break;
		button_req_sent = true;
		if (!buttons_linked) {
        	        strcpy(buttons_req_str, "{\"mac\":\"");
			strcat(buttons_req_str,mac_str);
        	        strcat(buttons_req_str, "\",\"device_type\":[\"room_sensor\"]");
        	        strcat(buttons_req_str, ",\"cmd\":\"link\"");
        	        strcat(buttons_req_str, ",\"token\":\"");
        	        strcat(buttons_req_str, token);
        	        strcat(buttons_req_str, "\"");
			strcat(buttons_req_str,"}");
			n = lws_snprintf((char *)p, sizeof(buttons_req_str) - LWS_PRE, "%s", buttons_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,buttons_req_str);
			}
			break;
		}
		touch_activated = false;
		strcpy(front_button_str,"[");
	        for (int i=0; i<TOUCH_PAD_MAX; i++) {
	            if (s_pad_activated_notify[i] == true) {
	                printf("Notify T%d!\n", i);
			sprintf(i_str,"%d",i);
			strcat(front_button_str,i_str);
   			strcat(front_button_str,",");
	                s_pad_activated_notify[i] = false;
			touch_activated = true;
	            }
	        }
		front_button_str[strlen(front_button_str)-1]=0;
		strcat(front_button_str,"]");
		if (!touch_activated) break;

                strcpy(buttons_req_str, "{\"mac\":\"");
		strcat(buttons_req_str,mac_str);
                strcat(buttons_req_str, "\",\"value\":");
                strcat(buttons_req_str, front_button_str);
                strcat(buttons_req_str, ",\"device_type\":[\"room_sensor\"]");
                strcat(buttons_req_str, ",\"cmd\":\"buttons\"");
                strcat(buttons_req_str, ",\"token\":\"");
                strcat(buttons_req_str, token);
                strcat(buttons_req_str, "\"");
		strcat(buttons_req_str,"}");


		n = lws_snprintf((char *)p, sizeof(buttons_req_str) - LWS_PRE, "%s", buttons_req_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			//printf("%s %s\n",tag,buttons_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;
		//strcpy(buttons_rx_data, (const char *)in);
		sprintf(button_command,"%s",(const char *)in);
		printf("%s %s\n", tag, button_command);
		if (strcmp(button_command,"link")) {
			printf("%s LINKED!!\n", tag);
			buttons_linked = true;
		}

		if (!strcmp(button_command,"light_on")) {
			printf("%s turining light on!\n", tag);
			light1_value = 100;
			light_on(LIGHT1,light1_value);
		}

		if (!strcmp(button_command,"light_off")) {
			printf("%s turining light off!\n", tag);
			light1_value = 0;
			light_on(LIGHT1,light1_value);
		}
		//button_req_sent = false;
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
