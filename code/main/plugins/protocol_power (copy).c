// ---------------------------  OPEN-AUTOMATION -------------------------------- //
// --------------  https://github.com/physiii/open-automation  ----------------- //
// --------------------------- protocol_power.c ------------------------------ //

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
#include "esp_adc_cal.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "driver/i2c.h"
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

#define VOLTAGE_CHANNEL (4)
#define CURRENT_CHANNEL (35)

#define ESP_INTR_FLAG_DEFAULT 0


/*Note: Different ESP32 modules may have different reference voltages varying from
 * 1000mV to 1200mV. Use #define GET_VREF to route v_ref to a GPIO
 */
#define V_REF   1100
#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_0)      //GPIO 34
//#define V_REF_TO_GPIO  //Remove comment on define to route v_ref to GPIO_ID

char temp_str[50];
bool power_received = false;
uint8_t mac[6];
char mac_str[20];
char power[256];
char previous_power[256];
char power_rx_data[256];
char power_command[100];
char power_str[100];
char i_str[10];
bool power_linked = false;
bool power_req_sent = false;
char power_req_str[1024];
int power_value = 0;
int main_voltage = 0;
int main_current = 0;
bool data_ready = false;
char current_str[10] = "";
char voltage_str[10] = "";

struct per_vhost_data__power {
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

struct per_session_data__power {
	int number;
	int value;
};

static void
uv_timeout_cb_power(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
		, int status
#endif
)
{
	struct per_vhost_data__power *vhd;
       
//	w = pvTimerGetTimerID((uv_timer_t)w);

	vhd = lws_container_of(w,
			struct per_vhost_data__power, timeout_watcher);

	if (vhd->vhost)
		lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

// ----------- //
// i2c actions //
// ----------- //

#define DATA_LENGTH          512  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH       127  /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define POWER_DELAY_TIME    2000 /*!< delay time between different test items */

#define I2C_EXAMPLE_MASTER_SCL_IO    19    /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO    18    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ   100000     /*!< I2C master clock frequency */

#define INA219_SENSOR_ADDR          0x41    /*!< slave address for SI7020 sensor */
#define INA219_CMD_MEASURE_VOLTAGE  0x02    /*!< Command to set measure mode */
#define INA219_CMD_MEASURE_CURRENT  0x01    /*!< Command to set measure mode */

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

xSemaphoreHandle print_mux;

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

static void i2c_example_master_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

static void power_task(void* arg)
{
    int i = 0;
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t sensor_data_h, sensor_data_l;

    while (1) {
        /*--------------------------------------------------*/
        ret = INA219_measure_current( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_OK) {
            //printf("data_h: %02x\n", sensor_data_h);
            //printf("data_l: %02x\n", sensor_data_l);
            //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
            printf("MEASURE CURRENT INA219 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
            sprintf(current_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
	    data_ready = true;
        } else {
            printf("INA219 No ack, sensor not connected...skip...\n");
        }

        xSemaphoreGive(print_mux);
        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);

        /*--------------------------------------------------*/
        ret = INA219_measure_voltage( I2C_EXAMPLE_MASTER_NUM, &sensor_data_h, &sensor_data_l);
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_OK) {
           //printf("data_h: %02x\n", sensor_data_h);
           //printf("data_l: %02x\n", sensor_data_l);
           //printf("sensor val: %f\n", ( sensor_data_h << 8 | sensor_data_l ) / 1.2);
           sprintf(voltage_str,"%d", ( sensor_data_h << 8 | sensor_data_l ));
           printf("MEASURE VOLTAGE INA219 (%d)\n", ( sensor_data_h << 8 | sensor_data_l ));
           data_ready = true;
        } else {
            printf("INA219 No ack, sensor not connected...skip...\n");
        }

        xSemaphoreGive(print_mux);
        vTaskDelay(( POWER_DELAY_TIME * ( task_idx + 1 ) ) / portTICK_RATE_MS);
    }
}

void start_i2c()
{
    print_mux = xSemaphoreCreateMutex();
    i2c_example_master_init();
    //xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void* ) 1, 10, NULL);
}


// ------------- //
// power actions //
// ------------- //

bool power_hold = false;

static int
callback_power(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char tag[50] = "[power-protocol]";
	struct per_session_data__power *pss =
			(struct per_session_data__power *)user;
	struct per_vhost_data__power *vhd =
			(struct per_vhost_data__power *)
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
		printf("%s initializing...\n",tag);
		//xTaskCreate(power_task, "power_task", 2048, NULL, 10, NULL);
		//init_gpio();
		//start_i2c();
		//xTaskCreate(power_task, "power_task", 1024 * 2, (void* ) 0, 10, NULL);
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
				lws_get_protocol(wsi),
				sizeof(struct per_vhost_data__power));
		vhd->context = lws_get_context(wsi);
		vhd->protocol = lws_vhost_name_to_protocol(lws_get_vhost(wsi),
					lws_get_protocol(wsi)->name);
		vhd->vhost = lws_get_vhost(wsi);

		uv_timer_init(lws_uv_getloop(vhd->context, 0),
			      &vhd->timeout_watcher);
		uv_timer_start(&vhd->timeout_watcher,
			       uv_timeout_cb_power, DUMB_PERIOD, DUMB_PERIOD);
		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
		if (!vhd)
			break;
	//	lwsl_notice("di: LWS_CALLBACK_PROTOCOL_DESTROY: v=%p, ctx=%p\n", vhd, vhd->context);
		uv_timer_stop(&vhd->timeout_watcher);
		uv_close((uv_handle_t *)&vhd->timeout_watcher, NULL);
		break;

	case LWS_CALLBACK_CLOSED:
		power_linked = false;
		break;


	case LWS_CALLBACK_ESTABLISHED:
		pss->number = 0;
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		printf("[LWS_CALLBACK_CLIENT_WRITEABLE] voltage_str: %d\n",voltage_str);
		break;
		if (!token_received) break;
		if (!data_ready) break;
		data_ready = false;
		//power_req_sent = true;
		if (!power_linked) {
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
				lwsl_err("ERROR %d writing to di socket\n", n);
			else  {
				printf("%s %s\n",tag,power_req_str);
			}
			break;
		}
		/*strcpy(power_str,"[");
		power_str[strlen(power_str)-1]=0;
		strcat(power_str,"]");
		break;*/
                strcpy(power_req_str, "{\"mac\":\"");
		strcat(power_req_str,mac_str);
                strcat(power_req_str, "\",\"voltage\":");
                strcat(power_req_str, voltage_str);
                strcat(power_req_str, ",\"current\":");
                strcat(power_req_str, current_str);
                strcat(power_req_str, ",\"device_type\":[\"regulator\"]");
                strcat(power_req_str, ",\"cmd\":\"power\"");
                strcat(power_req_str, ",\"token\":\"");
                strcat(power_req_str, token);
                strcat(power_req_str, "\"");
		strcat(power_req_str,"}");
		printf("%s %s\n",tag,power_req_str);
		break;
		n = lws_snprintf((char *)p, sizeof(power_req_str) - LWS_PRE, "%s", power_req_str);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) 
			lwsl_err("ERROR %d writing to di socket\n", n);
		else  {
			//printf("%s %s\n",tag,power_req_str);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		if (len < 2)
			break;
		//strcpy(power_rx_data, (const char *)in);
		sprintf(power_command,"%s",(const char *)in);
		//printf("%s %s\n", tag, power_command);
		if (strcmp(power_command,"link")) {
			//printf("%s LINKED!!\n", tag);
			power_linked = true;
		}

		if (!strcmp(power_command,"light_on")) {
			printf("%s turining power on!\n", tag);
                        //printf("%s setting power pin to %d to %d\n", tag, POWER_EN, power_value);
			power_value = 100;
                        gpio_set_level(POWER_EN, power_value);
			//power(POWER_EN,power_value);
		}

		if (!strcmp(power_command,"light_off")) {
			printf("%s turining power off!\n", tag);
			power_value = 0;
                        //printf("%s setting power pin to %d to %d\n", tag, POWER_EN, power_value);
                        gpio_set_level(POWER_EN, power_value);
			//power(POWER_EN,power_value);
		}
		//power_req_sent = false;
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
		1000, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)
		
static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_power
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
