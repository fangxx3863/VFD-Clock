#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "main.h"

void SERIAL_Setup();
void SERIAL1_Printf(const char *format, ...);
void SERIAL2_Printf(const char *format, ...);
void CDC_Printf(const char *format, ...);
void CDC_Write(char data[], uint32_t length);


__weak void SERIAL1_Proc(char* data, uint32_t len);
__weak void SERIAL2_Proc(char* data, uint32_t len);
// __weak void CDC_Proc(char* data, uint32_t len);
void CDC_Proc(char* data, uint32_t len);




#endif