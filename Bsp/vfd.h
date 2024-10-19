#ifndef __VFD_H__
#define __VFD_H__
#include "main.h"

/**
 * 初始化VFD
 */
void VFD_Init(void);

/**
 * 显示控制命令  COMMANDS 4
 * @param onOff 0显示关闭，1开启显示
 * @param brightnessVal 亮度占空比 0~7调整
 * 000：脉冲宽度= 1/16 0
 * 001：脉冲宽度= 2/16 1
 * 010 ：脉冲宽度= 4/16 0x2
 * 011 ：脉冲宽度= 10/16 3
 * 100：脉冲宽度= 11/16  4
 * 101 ：脉冲宽度= 12/16 0x5
 * 110：脉冲宽度= 13/16 6
 * 111：脉冲宽度= 14/16 0x7
 */
void VFD_SetDisplayLight(uint8_t onOff, uint8_t brightnessVal);
void VFD_SetModeWirteDisplayMode(uint8_t addressMode);
void VFD_SetDisplayMode(uint8_t digit);
void VFD_SendDigAndData(uint8_t dig, const uint8_t* data, size_t len);

// #define CLK_1 HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET)
// #define CLK_0 HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET)
// #define DIN_1 HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_SET)
// #define DIN_0 HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_RESET)
#define STB_1 HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_SET)
#define STB_0 HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET)


#endif // !__VFD_H__