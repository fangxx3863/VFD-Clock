#include "temp_humidity_task.h"
#include "cmsis_os.h"
#include <stdint.h>

dht11_t dht11;

// DHT11读取任务
void temp_humidity_task() {
    while (1) {
        DHT_ReadData(&dht11);
        osDelay(1000);
    }
}

// 获取温度和湿度
uint8_t DHT11_GetTemperature() { return dht11.temperature; }
uint8_t DHT11_GetHumidity() { return dht11.humidty; }