/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "cJSON.h"
#include <stdlib.h>
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
#define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
#define TEST_WITH_RELOAD      1        // testing will be done with auto reload

#define ZERO_DETECT_IO      35
#define TRIAC_IO      2
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;
#define GPIO_INPUT_PIN_SEL  ((1ULL<<ZERO_DETECT_IO))
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<TRIAC_IO))

int max_brightness = 255;
int zerocross_count = 0;
int current_brightness = 0;
double triac_delay = 0;
double min_triac_delay = 0.0001;
double max_triac_delay = 0.0075;
double main_wavelength = 5;

// sets delay from when zerocross is detected and
// triac is turned off to when the triac is turned
// on. A longer delay means less power available.
void set_brightness(int brightness) {
  current_brightness = brightness;
  if (brightness > max_brightness) brightness = max_brightness;
  if (brightness < 0) brightness = 0;
  brightness = max_brightness - brightness;
  triac_delay = min_triac_delay + max_triac_delay*((double)brightness/max_brightness);
  //triac_delay = max_triac_delay;
}

void dec_brightness(int amount) {
  int new_brightness = current_brightness - amount;
  set_brightness(new_brightness);
}

void inc_brightness(int amount) {
  int new_brightness = current_brightness + amount;
  set_brightness(new_brightness);
}

void fade_brightness(int start, int stop, int interval) {
  printf("fading from %d to %d in %dms\n",start,stop,interval);
  int divider = 3;
  interval*=divider;
  if (stop>start) {
    interval = interval / (stop - start);
    for(int i=start; i<=stop; i+=divider) {
      set_brightness(i);
      if (interval<10) interval=10;
      vTaskDelay( interval / portTICK_RATE_MS);
    }
  }

  if (start>stop) {
    interval = interval / (start - stop);
    for(int i=start; i>=stop; i-=divider) {
      set_brightness(i);
          if (interval<10) interval=10;
      vTaskDelay( interval / portTICK_RATE_MS);
    }
  }
}
/*
 * A sample structure to pass events
 * from the timer interrupt handler to the main program.
 */
typedef struct {
    int type;  // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

xQueueHandle timer_queue;

/*
 * A simple helper function to print the raw timer counter value
 * and the counter value converted to seconds
 */
static void inline print_timer_counter(uint64_t counter_value) {
    printf("Counter: 0x%08x%08x\n", (uint32_t) (counter_value >> 32),
                                    (uint32_t) (counter_value));
    printf("Time   : %.8f s\n", (double) counter_value / TIMER_SCALE);
}

// /*
//  * Timer group0 ISR handler
//  *
//  * Note:
//  * We don't call the timer API here because they are not declared with IRAM_ATTR.
//  * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
//  * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
//  */
// void IRAM_ATTR timer_group0_isr(void *para) {
//     int timer_idx = (int) para;
//
//     /* Retrieve the interrupt status and the counter value
//        from the timer that reported the interrupt */
//     uint32_t intr_status = TIMERG0.int_st_timers.val;
//     TIMERG0.hw_timer[timer_idx].update = 1;
//     uint64_t timer_counter_value =
//         ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
//         | TIMERG0.hw_timer[timer_idx].cnt_low;
//
//     /* Prepare basic event data
//        that will be then sent back to the main program task */
//     timer_event_t evt;
//     evt.timer_group = 0;
//     evt.timer_idx = timer_idx;
//     evt.timer_counter_value = timer_counter_value;
//
//     if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
//         TIMERG0.int_clr_timers.t0 = 1;
//         timer_counter_value += (uint64_t) (main_wavelength * TIMER_SCALE);
//         TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
//         TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
//         //gpio_set_level(TRIAC_IO, 1);
//         //TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
//     }
//
//     if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
//         gpio_set_level(TRIAC_IO, 1);
//         TIMERG0.int_clr_timers.t1 = 1;
//     }
//
//     //TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
//     //xQueueSendFromISR(timer_queue, &evt, NULL);
// }
//
// /*
//  * Timer group0 ISR handler
//  *
//  * Note:
//  * We don't call the timer API here because they are not declared with IRAM_ATTR.
//  * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
//  * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
//  */
// void IRAM_ATTR timer_group1_isr(void *para) {
//     int timer_idx = (int) para;
//
//     /* Retrieve the interrupt status and the counter value
//        from the timer that reported the interrupt */
//     uint32_t intr_status = TIMERG1.int_st_timers.val;
//     TIMERG1.hw_timer[timer_idx].update = 1;
//     uint64_t timer_counter_value =
//         ((uint64_t) TIMERG1.hw_timer[timer_idx].cnt_high) << 32
//         | TIMERG1.hw_timer[timer_idx].cnt_low;
//
//     /* Prepare basic event data
//        that will be then sent back to the main program task */
//     timer_event_t evt;
//     evt.timer_group = 0;
//     evt.timer_idx = timer_idx;
//     evt.timer_counter_value = timer_counter_value;
//
//     if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
//         TIMERG1.int_clr_timers.t0 = 1;
//         timer_counter_value += (uint64_t) (main_wavelength * TIMER_SCALE);
//         TIMERG1.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
//         TIMERG1.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
//         //gpio_set_level(TRIAC_IO, 1);
//         //TIMERG1.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
//     }
//
//
//     TIMERG1.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
//     //xQueueSendFromISR(timer_queue, &evt, NULL);
// }
//
// /*
//  * Initialize selected timer of the timer group 0
//  *
//  * timer_idx - the timer number to initialize
//  * auto_reload - should the timer auto reload on alarm?
//  * timer_interval_sec - the interval of alarm to set
//  */
// static void example_tg0_timer_init(int timer_idx,
//     bool auto_reload, double timer_interval_sec) {
//     /* Select and initialize basic parameters of the timer */
//     timer_config_t config;
//     config.divider = TIMER_DIVIDER;
//     config.counter_dir = TIMER_COUNT_UP;
//     config.counter_en = TIMER_PAUSE;
//     config.alarm_en = TIMER_ALARM_EN;
//     config.intr_type = TIMER_INTR_LEVEL;
//     config.auto_reload = auto_reload;
//     timer_init(TIMER_GROUP_0, timer_idx, &config);
//
//     /* Timer's counter will initially start from value below.
//        Also, if auto_reload is set, this value will be automatically reload on alarm */
//     timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);
//
//     /* Configure the alarm value and the interrupt on alarm. */
//     timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
//     timer_enable_intr(TIMER_GROUP_0, timer_idx);
//     timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
//         (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
//
//     timer_start(TIMER_GROUP_0, timer_idx);
// }
//
// static void example_tg1_timer_init(int timer_idx,
//     bool auto_reload, double timer_interval_sec) {
//     /* Select and initialize basic parameters of the timer */
//     timer_config_t config;
//     config.divider = TIMER_DIVIDER;
//     config.counter_dir = TIMER_COUNT_UP;
//     config.counter_en = TIMER_PAUSE;
//     config.alarm_en = TIMER_ALARM_EN;
//     config.intr_type = TIMER_INTR_LEVEL;
//     config.auto_reload = auto_reload;
//     timer_init(TIMER_GROUP_1, timer_idx, &config);
//
//     /* Timer's counter will initially start from value below.
//        Also, if auto_reload is set, this value will be automatically reload on alarm */
//     timer_set_counter_value(TIMER_GROUP_1, timer_idx, 0x00000000ULL);
//
//     /* Configure the alarm value and the interrupt on alarm. */
//     timer_set_alarm_value(TIMER_GROUP_1, timer_idx, timer_interval_sec * TIMER_SCALE);
//     timer_enable_intr(TIMER_GROUP_1, timer_idx);
//     timer_isr_register(TIMER_GROUP_1, timer_idx, timer_group0_isr,
//         (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
//
//     timer_start(TIMER_GROUP_1, timer_idx);
// }
//
// /*
//  * The main task of this example program
//  */
//  static void timer_example_evt_task(void *arg)
//  {
//      while (1) {
//          timer_event_t evt;
//          xQueueReceive(timer_queue, &evt, portMAX_DELAY);
//
//          /* Print information that the timer reported an event
//          if (evt.type == TEST_WITHOUT_RELOAD) {
//              printf("\n    Example timer without reload\n");
//          } else if (evt.type == TEST_WITH_RELOAD) {
//              printf("\n    Example timer with auto reload\n");
//          } else {
//              printf("\n    UNKNOWN EVENT TYPE\n");
//          }*/
//          printf("Group[%d], timer[%d] alarm event\n", evt.timer_group, evt.timer_idx);
//
//          /* Print the timer values passed by event */
//          printf("------- EVENT TIME --------\n");
//          print_timer_counter(evt.timer_counter_value);
//
//          /* Print the timer values as visible by this task */
//          printf("-------- TASK TIME --------\n");
//          uint64_t task_counter_value;
//          timer_get_counter_value(evt.timer_group, evt.timer_idx, &task_counter_value);
//          print_timer_counter(task_counter_value);
//      }
//  }

/*
 * Timer group0 ISR handler
 *
 * Note:
 * We don't call the timer API here because they are not declared with IRAM_ATTR.
 * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
 * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
 */
void IRAM_ATTR timer_group0_isr(void *para)
{
    int timer_idx = (int) para;

    /* Retrieve the interrupt status and the counter value
       from the timer that reported the interrupt */
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    TIMERG0.hw_timer[timer_idx].update = 1;
    uint64_t timer_counter_value =
        ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
        | TIMERG0.hw_timer[timer_idx].cnt_low;

    /* Prepare basic event data
       that will be then sent back to the main program task */
    timer_event_t evt;
    evt.timer_group = 0;
    evt.timer_idx = timer_idx;
    evt.timer_counter_value = timer_counter_value;

    /* Clear the interrupt
       and update the alarm time for the timer with without reload */
    if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
        evt.type = TEST_WITHOUT_RELOAD;
        TIMERG0.int_clr_timers.t0 = 1;
        timer_counter_value += (uint64_t) (main_wavelength * TIMER_SCALE);
        TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
        TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
    } else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
        evt.type = TEST_WITH_RELOAD;
        TIMERG0.int_clr_timers.t1 = 1;
    } else {
        evt.type = -1; // not supported even type
    }

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

    /* Now just send the event data back to the main program task */
    xQueueSendFromISR(timer_queue, &evt, NULL);
}

/*
 * Initialize selected timer of the timer group 0
 *
 * timer_idx - the timer number to initialize
 * auto_reload - should the timer auto reload on alarm?
 * timer_interval_sec - the interval of alarm to set
 */
static void example_tg0_timer_init(int timer_idx,
    bool auto_reload, double timer_interval_sec)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
        (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}

/*
 * The main task of this example program
 */
static void timer_example_evt_task(void *arg)
{
    while (1) {
        timer_event_t evt;
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);

        /* Print information that the timer reported an event */
        if (evt.type == TEST_WITHOUT_RELOAD) {
            printf("\n    Example timer without reload\n");
        } else if (evt.type == TEST_WITH_RELOAD) {
            printf("\n    Example timer with auto reload\n");
        } else {
            printf("\n    UNKNOWN EVENT TYPE\n");
        }
        printf("Group[%d], timer[%d] alarm event\n", evt.timer_group, evt.timer_idx);

        /* Print the timer values passed by event */
        printf("------- EVENT TIME --------\n");
        print_timer_counter(evt.timer_counter_value);

        /* Print the timer values as visible by this task */
        printf("-------- TASK TIME --------\n");
        uint64_t task_counter_value;
        timer_get_counter_value(evt.timer_group, evt.timer_idx, &task_counter_value);
        print_timer_counter(task_counter_value);
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;

    zerocross_count++;
    gpio_set_level(TRIAC_IO, 0);
    example_tg0_timer_init(TIMER_1, TEST_WITH_RELOAD, triac_delay);
}

void gpio_init() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

		io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_isr_handler_add(ZERO_DETECT_IO, gpio_isr_handler, (void*) ZERO_DETECT_IO);
}

void app_main() {
  timer_queue = xQueueCreate(10, sizeof(timer_event_t));
  example_tg0_timer_init(TIMER_0, TEST_WITHOUT_RELOAD, main_wavelength);
  example_tg0_timer_init(TIMER_1, TEST_WITH_RELOAD,    triac_delay);
  xTaskCreate(timer_example_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
    // gpio_init();
    // //timer_queue = xQueueCreate(10, sizeof(timer_event_t));
    // //xTaskCreate(timer_example_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
    // example_tg1_timer_init(TIMER_0, TEST_WITHOUT_RELOAD, main_wavelength);
    // while(1) {
    //     fade_brightness(0,110,3000);
    //     fade_brightness(110,0,3000);
    //
    // }
}
