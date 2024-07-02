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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"
#include "DC_MOTOR.h"
#include "HCSR04.h"
#include "Buzzer.h"
//#include "LCD.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "../../util/util.h"
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
void SysTick_CallBack(void);
void move_forward(void);
void move_backward(void);
void move_right(void);
void move_left(void);
void stop(void);
void UART_Receiving_Init(void);
void ObdtacleAvoidance_Logic(float Front_Distance, float Right_Distance, float Left_Distance);
void SafeDistance_Logic(float Safe_Distance);
void BlindSpot_Logic(float Right_Distance, float Left_Distance);
void UART_SendString(char *string);
void UART_SendFloat(float num);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define L298_ENA_Pin GPIO_PIN_1
#define L298_ENA_GPIO_Port GPIOA
#define L298_ENB_Pin GPIO_PIN_2
#define L298_ENB_GPIO_Port GPIOA
#define Echo3_Pin GPIO_PIN_6
#define Echo3_GPIO_Port GPIOA
#define Echo4_Pin GPIO_PIN_7
#define Echo4_GPIO_Port GPIOA
#define L298_IN1_Pin GPIO_PIN_0
#define L298_IN1_GPIO_Port GPIOB
#define L298_IN2_Pin GPIO_PIN_1
#define L298_IN2_GPIO_Port GPIOB
#define Trigger3_Pin GPIO_PIN_10
#define Trigger3_GPIO_Port GPIOB
#define Trigger4_Pin GPIO_PIN_12
#define Trigger4_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_13
#define Buzzer_GPIO_Port GPIOB
#define Trigger1_Pin GPIO_PIN_11
#define Trigger1_GPIO_Port GPIOA
#define Trigger2_Pin GPIO_PIN_12
#define Trigger2_GPIO_Port GPIOA
#define L298_IN3_Pin GPIO_PIN_4
#define L298_IN3_GPIO_Port GPIOB
#define L298_IN4_Pin GPIO_PIN_5
#define L298_IN4_GPIO_Port GPIOB
#define Echo1_Pin GPIO_PIN_6
#define Echo1_GPIO_Port GPIOB
#define Echo2_Pin GPIO_PIN_7
#define Echo2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
