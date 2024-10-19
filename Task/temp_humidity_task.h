#ifndef __TEMP_HUMIDITY_TASK_H__
#define __TEMP_HUMIDITY_TASK_H__
#include "main.h"
#include "dht.h"

extern dht11_t dht11;

void temp_humidity_task();
uint8_t DHT11_GetTemperature();
uint8_t DHT11_GetHumidity();

#endif // !__TEMP_HUMIDITY_TASK_H__