/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "sdkconfig.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp_chip_info.h"
#include "driver/gptimer.h"
#include "esp_timer.h"
#include "sk6812_led.h"

enum TARGET {
    TARGET1,
    TARGET2
} target = TARGET1;

ColourState state1{100, 100, 100};
ColourState state2{10, 255, 10};
ColourState state{0,0,0};

static bool timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    if(target == TARGET1)  target = TARGET2;
    else target = TARGET1;

    return true;
}

extern "C" {
void app_main(void) {
    sk6812_ledInit();

    // timer
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
            .clk_src = GPTIMER_CLK_SRC_DEFAULT,
            .direction = GPTIMER_COUNT_UP,
            .resolution_hz = 100000,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_event_callbacks_t cbs = {
            .on_alarm = timer_on_alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    gptimer_alarm_config_t alarm_config = {
            200000, // period = 2s
            0, // counter will reload with 0 on alarm event
            {true}, // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    while (true) {
//          int i = 0;
//        //     green
//        for( i=0; i<8; i++) {
////            setT1H();
////            setT1L();
//            setT0H();
//            setT0L();
//        }
//
//        // red
//        for(; i<16; i++) {
////            setT1H();
////            setT1L();
//            setT0H();
//            setT0L();
//        }
//
//        // blue
//        for(; i<24; i++) {
//            setT1H();
//            setT1L();
////            setT0H();
////            setT0L();
//        }
//        setT0L();


        if(target == TARGET1)   state.stepTo(state1);
        else    state.stepTo(state2);

        sk6812Shine(state);
        
        vTaskDelay(1);
    }
}
}
