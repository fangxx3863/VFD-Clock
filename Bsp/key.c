#include "key.h"
#define KEY_COUNT 2  // 按键数量
#define DEBOUNCE_TIME   50  // 去抖时间（毫秒）
#define LONG_PRESS_TIME 500 // 长按时间（毫秒）

typedef struct {
    GPIO_TypeDef* port;    // 按键连接的GPIO端口
    uint16_t pin;          // 按键连接的GPIO引脚
    uint8_t state;         // 按键当前状态
    uint8_t short_press;   // 短按标志
    uint8_t long_press;    // 长按标志
    uint32_t timestamp;    // 上次状态变化的时间戳
} Key_TypeDef;

Key_TypeDef keys[KEY_COUNT] = {
	{B1_GPIO_Port, B1_Pin},
	{B2_GPIO_Port, B2_Pin},
    // ... 其他按键
};

typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_PRESS,
    KEY_STATE_LONG_PRESS
} KeyState_TypeDef;

void KEY_Proc(void) {
    for (int i = 0; i < KEY_COUNT; i++) {
        Key_TypeDef *key = &keys[i];
        uint8_t current_state = HAL_GPIO_ReadPin(key->port, key->pin);
        uint32_t current_time = HAL_GetTick();

        switch (key->state) {
            case KEY_STATE_IDLE:
                if (current_state == GPIO_PIN_RESET) {
                    key->timestamp = current_time;
                    key->state = KEY_STATE_PRESS;
                }
                break;

            case KEY_STATE_PRESS:
                /*if (current_state != GPIO_PIN_RESET) {
                    // 误触或短按释放
                    key->state = KEY_STATE_IDLE;
                } else */
				if (current_time - key->timestamp >= DEBOUNCE_TIME) {
					if (current_state == GPIO_PIN_SET) {
						key->short_press = 1;
						key->state = KEY_STATE_IDLE;
					}
                    // 等待长按
                    if (current_time - key->timestamp >= LONG_PRESS_TIME) {
                        key->state = KEY_STATE_LONG_PRESS;
                    }
                }
                break;

            case KEY_STATE_LONG_PRESS:
                // 长按状态下，如果按键释放，则设置长按标志位
                if (current_state == GPIO_PIN_SET) {
                    key->long_press = 1;
                    key->state = KEY_STATE_IDLE;
                }
                break;

            default:
                // 默认情况下回到空闲状态
                key->state = KEY_STATE_IDLE;
                break;
        }
    }
}

uint8_t KEY_GetClick(uint8_t index) {
    if (index < KEY_COUNT && keys[index].short_press) {
        keys[index].short_press = 0;
        return 1;
    }
    return 0;
}

uint8_t KEY_GetHold(uint8_t index) {
    if (index < KEY_COUNT && keys[index].long_press) {
        keys[index].long_press = 0;
        return 1;
    }
    return 0;
}
