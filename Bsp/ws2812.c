#include "ws2812.h"
#include "tim.h"

/* WS2812B f=800k, T=1.25us */
#define ONE_PULSE    (40) //1 码 (2/3*T)
#define ZERO_PULSE   (20)  //0 码 (1/3*T)
#define RESET_PULSE  (150) //低电平复位信号50us
#define LED_DATA_LEN (24)  //led 颜色数据长度, 一个灯珠需要24bits
#define WS2812_DATA_LEN (RESET_PULSE + LED_NUMS * LED_DATA_LEN) //ws2812灯条需要的总数组长度

/* !! 若嵌套循环使用, 须注意变量(i) !! */
#define LOOP_ALL for(size_t i = 0; i < LED_NUMS; i++) /* 所有灯珠 */

#define DEFAULT_BRIGHTNESS (15) //灯带默认亮度: 0~100

static uint16_t RGB_buffer[WS2812_DATA_LEN] = {0};
static void color_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);


/* ************************************ Public Functions ************************************ */

/**
* @brief WS2812初始化, 全黑
*/
void WS2812_Setup(void)
{
    WS2812_SetDark(0);
}

/**
* @brief 设置某个灯珠颜色RGB
*
* @param uint8_t R,G,B: RGB色彩格式 红,绿,蓝通道数据
*        uint16_t num, 指定设置颜色的灯珠位号
*/
void WS2812_SetRGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    if (num > LED_NUMS)
        return;

    uint16_t *p = (RGB_buffer + RESET_PULSE) + (num * LED_DATA_LEN);

    for (uint16_t i = 0; i < 8; i++)
    {
        p[i]      = (G << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 8]  = (R << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
    }
}

/**
* @brief 设置某个灯珠颜色HSV
*
* @param uint8_t H,S,V: HSV色彩格式
*        uint16_t num, 指定设置颜色的灯珠位号
*/
void WS2812_SetHSV(uint16_t H, uint16_t S, uint16_t V, uint16_t num)
{
    uint32_t R = 0, G = 0, B = 0;

    if (num > LED_NUMS)
        return;

    uint16_t *p = (RGB_buffer + RESET_PULSE) + (num * LED_DATA_LEN);

    color_hsv2rgb(H, S, V, &R, &G, &B);

    for (uint16_t i = 0; i < 8; i++)
    {
        p[i]      = (G << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 8]  = (R << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
    }
}

/**
* @brief 灭灯
*/
void WS2812_SetDark(uint8_t type)
{
    switch (type)
    {
    default:
        LOOP_ALL
        {
            WS2812_SetRGB(0x00, 0x00, 0x00, i);
        }
        break;
    }

    WS2812_Refresh();
}

/**
* @brief WS2812全彩渐变
* @note 需放置循环体内
*/
void WS2812_ColorfulShadow(void)
{
	static volatile uint32_t lastUpdate = 0;
    static uint16_t color = 0; // 定义为静态变量，以保持其在函数调用之间的值
    static uint8_t i = 0;      // 定义为静态变量，以保持其在函数调用之间的值

    // 获取当前时间戳
    uint32_t now = HAL_GetTick();

    // 检查是否已经过了50毫秒
    if (now - lastUpdate >= 40)
    {
        lastUpdate = now; // 更新时间戳

        // 设置LED颜色
        WS2812_SetHSV(color, 100, DEFAULT_BRIGHTNESS, i);

        // 更新灯带
        WS2812_Refresh();

        // 准备下一个颜色
        i++;
        if (i >= LED_NUMS)
        {
            i = 0;
            color++;
            if (color >= 360)
            {
                color = 0; // 重置颜色
            }
        }
    }
}


/* ************************************ Static Functions ************************************ */

/**
* @brief WS2812颜色数据刷新, 修改颜色值后调用
*/
void WS2812_Refresh(void)
{
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3, (uint32_t *)RGB_buffer, WS2812_DATA_LEN);
}

/**
 * @brief 将HSV颜色空间转换为RGB颜色空间
 *
 * @param  h HSV颜色空间的H：色调, 范围0~360
 * @param  s HSV颜色空间的S：饱和度, 范围0~100
 * @param  v HSV颜色空间的V：明度, 范围0~100
 * @param  r 转换后RGB-R值的指针
 * @param  g 转换后RGB-G值的指针
 * @param  b 转换后RGB-B值的指针
 *
 */
static void color_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i)
    {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

