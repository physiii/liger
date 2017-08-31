// ---------------------------  OPEN-AUTOMATION -------------------------------- //
// --------------  https://github.com/physiii/open-automation  ----------------- //
// --------------------------- protocol_LED.c ------------------------------ //

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

#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "esp_log.h"

#include "freertos/xtensa_api.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "esp_attr.h"   
#include "esp_err.h"
#if defined(LWS_WITH_ESP8266)
#define DUMB_PERIOD 50
#else
#define DUMB_PERIOD 50
#endif

#define SAMPLE_SIZE (128)
#define SAMPLE_RATE (44100)



#define LED_R     18
#define LED_G     19
#define LED_B     5

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;

char temp_str[50];
bool LED_received = false;
uint8_t mac[6];
char mac_str[20];
char LED[256];
char previous_LED[256];
char LED_rx_data[256];
char LED_command[100];
char front_LED_str[100] = "init";
char i_str[10];
bool LED_linked = false;
bool LED_req_sent = false;
char LED_str[250] = "";
char LED_req_str[1024];

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (18)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_HS_CH1_GPIO       (19)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (4)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (5)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3

#define LEDC_TEST_CH_NUM       (4)
typedef struct {
    int channel;
    int io;
    int mode;
    int timer_idx;
} ledc_info_t;

static void LED_task(void* arg) {
//void LED_task() {
    int ch;
    ledc_info_t ledc_ch[LEDC_TEST_CH_NUM] = {
        {
            .channel   = LEDC_HS_CH0_CHANNEL,
            .io        = LEDC_HS_CH0_GPIO,
            .mode      = LEDC_HS_MODE,
            .timer_idx = LEDC_HS_TIMER
        },
        {
            .channel   = LEDC_HS_CH1_CHANNEL,
            .io        = LEDC_HS_CH1_GPIO,
            .mode      = LEDC_HS_MODE,
            .timer_idx = LEDC_HS_TIMER
        },
        {
            .channel   = LEDC_LS_CH2_CHANNEL,
            .io        = LEDC_LS_CH2_GPIO,
            .mode      = LEDC_LS_MODE,
            .timer_idx = LEDC_LS_TIMER
        },
        {
            .channel   = LEDC_LS_CH3_CHANNEL,
            .io        = LEDC_LS_CH3_GPIO,
            .mode      = LEDC_LS_MODE,
            .timer_idx = LEDC_LS_TIMER
        }
    };

    ledc_timer_config_t ledc_timer = {
        .bit_num = LEDC_TIMER_13_BIT, //set timer counter bit number
        .freq_hz = 5000,              //set frequency of pwm
        .speed_mode = LEDC_HS_MODE,   //timer mode,
        .timer_num = LEDC_HS_TIMER    //timer index
    };
    //configure timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    //configure timer1 for low speed channels
    ledc_timer.speed_mode = LEDC_LS_MODE;
    ledc_timer.timer_num = LEDC_LS_TIMER;
    ledc_timer_config(&ledc_timer);

    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config_t ledc_channel = {
            //set LEDC channel 0
            .channel = ledc_ch[ch].channel,
            //set the duty for initialization.(duty range is 0 ~ ((2**bit_num)-1)
            .duty = 0,
            //GPIO number
            .gpio_num = ledc_ch[ch].io,
            //GPIO INTR TYPE, as an example, we enable fade_end interrupt here.
            .intr_type = LEDC_INTR_FADE_END,
            //set LEDC mode, from ledc_mode_t
            .speed_mode = ledc_ch[ch].mode,
            //set LEDC timer source, if different channel use one timer,
            //the frequency and bit_num of these channels should be the same
            .timer_sel = ledc_ch[ch].timer_idx,
        };
        //set the configuration
        ledc_channel_config(&ledc_channel);
    }

    //initialize fade service.
    ledc_fade_func_install(0);
    while (1) {
        //printf("green to %d\n",LED_G_value);
        //for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        //}

        ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, LED_R_value);
        ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
        ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL, LED_G_value);
        ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL);
        ledc_set_duty(LEDC_HS_MODE, LEDC_LS_CH2_CHANNEL, LED_B_value);
        ledc_update_duty(LEDC_HS_MODE, LEDC_LS_CH2_CHANNEL);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        /*printf("fade up\n");
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_fade_with_time(ledc_ch[ch].mode, ledc_ch[ch].channel, 0, 2000);
            ledc_fade_start(ledc_ch[ch].mode, ledc_ch[ch].channel, LEDC_FADE_NO_WAIT);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        printf("set duty without fade\n");
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_duty(ledc_ch[ch].mode, ledc_ch[ch].channel, 8000);
            ledc_update_duty(ledc_ch[ch].mode, ledc_ch[ch].channel);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);*/
    }
}


struct per_vhost_data__LED {
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

struct per_session_data__LED {
	int number;
	int value;
};

static void
uv_timeout_cb_LED(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__LED *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__LED, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

// ----------- //
// led actions //
// ----------- //

static void LED_fade(int velocity, bool direction) {

}


static int
callback_LED(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[LED-protocol]";

	struct per_session_data__LED *pss =
			(struct per_session_data_LED *)user;
	struct per_vhost_data__LED *vhd =
			(struct per_vhost_data_LED *)
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
		// LED_ init //
		// -----------//
		xTaskCreate(LED_task, "LED_task", 2048, NULL, 10, NULL);
		//LED_task();
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__LED));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_LED, DUMB_PERIOD, DUMB_PERIOD);
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
		//if (LED_req_sent) break;
		LED_req_sent = true;
		if (!LED_linked) {
        	        strcpy(LED_req_str, "{\"mac\":\"");
			strcat(LED_req_str,mac_str);
        	        strcat(LED_req_str, "\",\"device_type\":[\"room_sensor\"]");
        	        strcat(LED_req_str, ",\"cmd\":\"link\"");
        	        strcat(LED_req_str, ",\"token\":\"");
        	        strcat(LED_req_str, token);
        	        strcat(LED_req_str, "\"");
			strcat(LED_req_str,"}");
			n = lws_snprintf((char *)p, sizeof(LED_req_str) - LWS_PRE, "%s", LED_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,LED_req_str);
			}
			break;
		}

		break;
                strcpy(LED_req_str, "{\"mac\":\"");
		strcat(LED_req_str,mac_str);
                strcat(LED_req_str, "\",\"value\":\"");
                strcat(LED_req_str, front_LED_str);
                strcat(LED_req_str, "\",\"device_type\":[\"room_sensor\"]");
                strcat(LED_req_str, ",\"cmd\":\"LED\"");
                strcat(LED_req_str, ",\"token\":\"");
                strcat(LED_req_str, token);
                strcat(LED_req_str, "\"");
		strcat(LED_req_str,"}");


		n = lws_snprintf((char *)p, sizeof(LED_req_str) - LWS_PRE, "%s", LED_req_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			//printf("%s %s\n",tag,LED_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;
		//strcpy(LED_rx_data, (const char *)in);
		sprintf(LED_command,"%s",(const char *)in);
		printf("%s %s\n", tag, LED_command);
		if (strcmp(LED_command,"link")) {
			printf("%s LINKED\n", tag);
			LED_linked = true;
		}

		if (!strcmp(LED_command,"LED_on")) {
			printf("%s turning LED on!\n", tag);
			LED_G_value = 100;
		        //ledc_set_duty(ledc_ch[ch].mode, ledc_ch[ch].channel, 0);
			//LED_set(LED_R,LED_R_value);
		}

		if (!strcmp(LED_command,"LED_off")) {
			printf("%s turning LED off!\n", tag);
			LED_G_value = 0;
		        //ledc_set_duty(ledc_ch[ch].mode, ledc_ch[ch].channel, 8000);
			//LED_set(LED_R,LED_R_value);
		}
		//LED_req_sent = false;
		break;

	default:
	   	printf("%s callback_LED: %d\n",tag,reason);
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_LED \
	{ \
		"LED-protocol", \
		callback_LED, \
		sizeof(struct per_session_data__LED), \
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_LED
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_LED(struct lws_context *context,
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
destroy_protocol_LED(struct lws_context *context)
{
	return 0;
}

#endif
