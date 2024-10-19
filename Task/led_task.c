#include "led_task.h"
#include "ws2812.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "tim.h"

extern osThreadId_t ledTaskHandle;
static int _led_en = 1;

void led_task() {
    while (1) {
        osDelay(40);
        if (!_led_en) {
            WS2812_SetDark(0);
            osThreadSuspend(ledTaskHandle);
        }else {
            WS2812_ColorfulShadow();

        }
    }
}

void led_en(int en) {
    if (en) {
        _led_en = 1;
        WS2812_SetDark(0);
        osThreadResume(ledTaskHandle);
    }else {
        _led_en = 0;
    }
}