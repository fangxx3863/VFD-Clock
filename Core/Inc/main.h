/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOA
#define WS2812_POWER_Pin GPIO_PIN_3
#define WS2812_POWER_GPIO_Port GPIOA
#define STB_Pin GPIO_PIN_4
#define STB_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_5
#define CLK_GPIO_Port GPIOA
#define DIN_Pin GPIO_PIN_7
#define DIN_GPIO_Port GPIOA
#define WS2812_Pin GPIO_PIN_0
#define WS2812_GPIO_Port GPIOB
#define ITR_RX_Pin GPIO_PIN_1
#define ITR_RX_GPIO_Port GPIOB
#define VFD_POWER_Pin GPIO_PIN_10
#define VFD_POWER_GPIO_Port GPIOB
#define DHT11_POWER_Pin GPIO_PIN_11
#define DHT11_POWER_GPIO_Port GPIOB
#define B2_Pin GPIO_PIN_12
#define B2_GPIO_Port GPIOB
#define ITR_TX_Pin GPIO_PIN_13
#define ITR_TX_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_15
#define DHT11_GPIO_Port GPIOB
#define VFD_PWM_Pin GPIO_PIN_8
#define VFD_PWM_GPIO_Port GPIOA
#define BAT_FULL_Pin GPIO_PIN_3
#define BAT_FULL_GPIO_Port GPIOB
#define BAT_CHRG_Pin GPIO_PIN_4
#define BAT_CHRG_GPIO_Port GPIOB
#define DS_IRQ_Pin GPIO_PIN_5
#define DS_IRQ_GPIO_Port GPIOB
#define DS_RST_Pin GPIO_PIN_8
#define DS_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
