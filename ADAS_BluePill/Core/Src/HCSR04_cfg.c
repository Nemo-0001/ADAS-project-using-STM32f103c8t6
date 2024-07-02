/*
 * File: HCSR04_cfg.c
 * Driver Name: [ HC-SR04 Ultrasonic Sensor ]
 * SW Layer:   ECUAL
 * Created on: 16/2/2024
 * Author:     Abdallah Alnemr
 *
 */

#include "HCSR04.h"

const HCSR04_CfgType HCSR04_CfgParam[HCSR04_UNITS] =
{
	// HC-SR04 Sensor Unit 1 Configurations
    {
		GPIOA,         //Trigger pin1 -->> A11
		GPIO_PIN_11,
		TIM4,
		TIM_CHANNEL_1, //Echo pin1 -->> B6
		72
	},
	// HC-SR04 Sensor Unit 2 Configurations
	{
		GPIOA,        //Trigger pin2 -->> A12
		GPIO_PIN_12,
		TIM4,
		TIM_CHANNEL_2, //Echo pin2 -->> B7
		72
	},
	// HC-SR04 Sensor Unit 3 Configurations
    {
		GPIOB,         //Trigger pin3 -->> B10
		GPIO_PIN_10,
		TIM3,
		TIM_CHANNEL_1, //Echo pin3 -->> A6
		72
	},
	// HC-SR04 Sensor Unit 4 Configurations
	{
		GPIOB,         //Trigger pin4 -->> B12
		GPIO_PIN_12,
		TIM3,
		TIM_CHANNEL_2, //Echo pin3 -->> A7
		72
	}
};
