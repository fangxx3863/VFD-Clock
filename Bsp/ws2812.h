#ifndef __WS2812_H__
#define __WS2812_H__
#include "stdint.h"
#include "main.h"

#define LED_NUMS (3) //灯珠数量

/* HSV格式常用色值预设 */
#define RED 0
#define YELLOW 60
#define GREEN 120
#define CYAN 180
#define BLUE 240

void WS2812_Setup(void);
void WS2812_SetRGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);
void WS2812_Refresh(void);
void WS2812_SetHSV(uint16_t H, uint16_t S, uint16_t V, uint16_t num);
void WS2812_SetDark(uint8_t type);
void WS2812_ColorfulShadow(void);

#endif  //__WS2812_H__

