#include "temp_humidity_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "vfd.h"
#include "gfx.h"

dht11_t dht11;

void temp_humidity_task() {
    while (1) {
        DHT_ReadData(&dht11);
        osDelay(1000);
    }
}

uint8_t DHT11_GetTemperature() { return dht11.temperature; }
uint8_t DHT11_GetHumidity() { return dht11.humidty; }