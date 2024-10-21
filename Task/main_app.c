#include "main_app.h"
// 标准库
#include <stdio.h>
// HAL库
#include "main.h"
#include "tim.h"
#include "i2c.h"
#include "adc.h"
// RTOS库
#include "cmsis_os.h"
// BSP库
#include "vfd.h"
#include "gfx.h"
#include "dht.h"
#include "ds3231.h"
#include "key.h"
#include "ws2812.h"
// 任务
#include "temp_humidity_task.h"
#include "led_task.h"

extern osMutexId_t vfdIconMutexHandle;

void periph_init() {
    VFD_Init();     // VFD初始化
    VFD_GfxInit();  // VFD图形库初始化
    HAL_GPIO_WritePin(DHT11_POWER_GPIO_Port, DHT11_POWER_Pin, GPIO_PIN_RESET);  // 打开DHT11电源
    DHT_Init(&dht11, &htim2, DHT11_GPIO_Port, DHT11_Pin);   // DHT11初始化
    DS3231_Init(&hi2c1);    // DS3231初始化
    HAL_GPIO_WritePin(WS2812_POWER_GPIO_Port, WS2812_POWER_Pin, GPIO_PIN_RESET);    // 打开WS2812电源
    WS2812_Setup();   // WS2812初始化
}

void main_app() {
    static int page = 0, ws2812_en = 1, brightness = 7;    // 定义静态变量
    while (1) {     // 主循环
        if (KEY_GetHold(B2)) {      // 处理B2长按事件
            // 开关WS2812
            if (ws2812_en) {
                ws2812_en = !ws2812_en;
                led_en(0);
                HAL_GPIO_WritePin(WS2812_POWER_GPIO_Port, WS2812_POWER_Pin, GPIO_PIN_SET);
                VFD_GfxSetText("LEDOFF");
                HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
                osDelay(10);
                HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
                osDelay(500);
            }else {
                ws2812_en = !ws2812_en;
                led_en(1);
                HAL_GPIO_WritePin(WS2812_POWER_GPIO_Port, WS2812_POWER_Pin, GPIO_PIN_RESET);
                VFD_GfxSetText("LED ON");
                HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
                osDelay(10);
                HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
                osDelay(500);
            }
        }
        if (KEY_GetHold(B1)) {      // 处理B1长按事件
            // 调节VFD亮度
            brightness++;
            if (brightness > 7) {
                brightness = 1;
            }
            if (brightness < 1) {
                brightness = 7;
            }
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
            osDelay(10);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
            VFD_GfxSetBlkLevel(brightness);
        }
        if (KEY_GetClick(B2)) {     // 处理B2短按事件
            // 切换页面
            page--;
            if (page < 0) {
                page = 4;
            }
            VFD_GfxSetIcon(ICON_NONE, 0);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
            osDelay(5);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
        }
        if (KEY_GetClick(B1)) {     // 处理B1短按事件
            // 切换页面
            page++;
            if (page > 4) {
                page = 0;
            }
            VFD_GfxSetIcon(ICON_NONE, 0);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
            osDelay(5);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
        }
        // 渲染页面
        switch (page) {
            case 0:
                clock_page();
                break;
            case 1:
                date_page();
                break;
            case 2:
                temperature_page();
                break;
            case 3:
                humidity_page();
                break;
            case 4:
                battery_page();
                break;
        }
        osDelay(20);
    }
}

// 时钟页面
void clock_page() {
    char tmp[24] = {0};
    static int cnt = 0;
    static int past_sec = -1;
    static int rec_clr_flg = 0;
    // 获取DS3231秒钟
    int sec = DS3231_GetSecond();
    // 处理REC图标闪烁
    if (rec_clr_flg == 1) {
        osMutexWait(vfdIconMutexHandle, osWaitForever);
        VFD_GfxSetIcon(VFD_GfxGetSaveIcon(), 0);
        rec_clr_flg = 0;
        osMutexRelease(vfdIconMutexHandle);
    }
    // 处理时间显示
    sprintf(tmp, "%02d%02d%02d", DS3231_GetHour(), DS3231_GetMinute(), sec);
    VFD_GfxSetText(tmp);
    VFD_GfxSetColon1(1);
    VFD_GfxSetColon2(1);
    // 处理G1动画效果
    if (cnt++ > 6) {
        osMutexWait(vfdIconMutexHandle, osWaitForever);
        VFD_GfxAnnoForG1(0);
        cnt = 0;
        osMutexRelease(vfdIconMutexHandle);
    }
    // 处理REC图标闪烁
    if (sec != past_sec) {
        osMutexWait(vfdIconMutexHandle, osWaitForever);
        VFD_GfxSetIcon(VFD_GfxGetSaveIcon() | ICON_REC, 0);
        past_sec = sec;
        rec_clr_flg = 1;
        osMutexRelease(vfdIconMutexHandle);
    }
    
}

// 日期页面
void date_page() {
    char tmp[24] = {0};
    // 单独处理年份后两位显示
    int year = DS3231_GetYear();
    sprintf(tmp, "%02d%02d%02d", ((year / 10) % 10) * 10 + (year % 10), (int)DS3231_GetMonth(), (int)DS3231_GetDate());
    VFD_GfxSetText(tmp);
}

// 温度页面
void temperature_page() {
    char tmp[24] = {0};
    sprintf(tmp, "T:%d C", DHT11_GetTemperature());
    VFD_GfxSetText(tmp);
}

// 湿度页面
void humidity_page() {
    char tmp[24] = {0};
    sprintf(tmp, "H:%d %%", DHT11_GetHumidity());
    VFD_GfxSetText(tmp);
}

// 电池电压页面
void battery_page() {
    static float battery_voltage = 0;
    static float adc_tmp = 0, adc_cnt = 0;
    // 读取电池电压并计算平均值(滤波)
    HAL_ADC_Start(&hadc1);
    adc_tmp += HAL_ADC_GetValue(&hadc1)*(4.2/4095);
    adc_cnt++;
    if (adc_cnt > 5) {
        battery_voltage = adc_tmp / adc_cnt;
        adc_tmp = 0;
        adc_cnt = 0;
    }
    char tmp[24] = {0};
    // 显示电池电压
    sprintf(tmp, "B:%.2f", battery_voltage);
    VFD_GfxSetText(tmp);
    // 充电中显示G1动画
    if (HAL_GPIO_ReadPin(BAT_CHRG_GPIO_Port, BAT_CHRG_Pin) == 0 && adc_cnt == 6) {
        VFD_GfxAnnoForG1(0);
    }
    // 充满电显示G1全亮
    if (HAL_GPIO_ReadPin(BAT_FULL_GPIO_Port, BAT_FULL_Pin) == 0 && adc_cnt == 6) {
        VFD_GfxSetIcon(ICON_G1_ALL, 0);
    }
}