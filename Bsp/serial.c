#include "serial.h"
#include "usart.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
#include "string.h"
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include "ds3231.h"

void convert_timestamp_to_ymdhms(uint32_t timestamp, int *year, int *month, int *day, int *hour, int *minute, int *second) {
    // 将时间戳转换为tm结构
    time_t t = (time_t)timestamp;
    struct tm *timeinfo = localtime(&t);

    // 从tm结构中提取年月日时分秒
    *year = timeinfo->tm_year + 1900; // tm_year是从1900年开始的年数
    *month = timeinfo->tm_mon + 1;    // tm_mon是从0开始的月份，所以要加1
    *day = timeinfo->tm_mday;
    *hour = timeinfo->tm_hour;
    *minute = timeinfo->tm_min;
    *second = timeinfo->tm_sec;
}

char uart1_receive_data[64];
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

void SERIAL_Setup() {
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)&uart1_receive_data, 64);
	
}


void CDC_Printf(const char *format, ...) {
    va_list args;
    uint32_t length;
 
    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
    va_end(args);
    CDC_Transmit_FS(UserTxBufferFS, length);
}

void CDC_Write(char data[], uint32_t length) {
	CDC_Transmit_FS(UserTxBufferFS, length);
}

void SERIAL1_Printf(const char *format, ...) {
    va_list args;
    uint32_t length;
	uint8_t UserTxBufferFS[64];
 
    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, sizeof(UserTxBufferFS), (char *)format, args);
    va_end(args);
	HAL_UART_Transmit(&huart1, UserTxBufferFS, length, 5000);
}



void SERIAL1_Proc(char* data, uint32_t len) {	// UART1串口解码
	UNUSED(data);
	UNUSED(len);
}


void CDC_Proc(char* data, uint32_t len) {	    // CDC串口解码
	// UNUSED(data);
	// UNUSED(len);
	data[len+1] = '\0';
	char cmd[10] = {0};
	sscanf(data, "%s", cmd);
	// CDC_Printf(cmd);
	if (strcmp(cmd, "timeset") == 0) {
		int year, month, day, hour, minute, second;
		uint32_t timestamp;
		sscanf(data, "%s %d", cmd, &timestamp);
		convert_timestamp_to_ymdhms(timestamp, &year, &month, &day, &hour, &minute, &second);
		DS3231_SetYear(year);
		DS3231_SetMonth(month);
		DS3231_SetDate(day);
		DS3231_SetHour(hour);
		DS3231_SetMinute(minute);
		DS3231_SetSecond(second);
		CDC_Printf("%s\r\nTime set to %d-%02d-%02d %02d:%02d:%02d\r\n", data, year, month, day, hour, minute, second);
	}
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    // 在这里处理接收到的数据
	if (huart->Instance == USART1) {
		SERIAL1_Proc(uart1_receive_data, Size);
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)&uart1_receive_data, 64);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)uart1_receive_data, 64);
	}
}