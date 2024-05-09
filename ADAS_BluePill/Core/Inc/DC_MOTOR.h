/*
 * File: DC_MOTOR.h
 * Driver Name: [ DC MOTOR ]
 * SW Layer:   ECUAL
 * Created on: 15/2/2024
 * Author:     Abdallah Alnemr
 *
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"

// DC Motor Rotation Directions
#define DIR_CW    0
#define DIR_CCW   1

// DC Motor PWM Properties
#define DC_MOTOR_PWM_RES  10
#define DC_MOTOR_F_PWM    500

// The Number Of DC Motors to be used in the project
#define DC_MOTOR_UNITS  2

typedef struct
{
	GPIO_TypeDef * DIR1_GPIO;
	GPIO_TypeDef * DIR2_GPIO;
	uint16_t       DIR1_PIN;
	uint16_t       DIR2_PIN;
	TIM_TypeDef*   TIM_Instance;
	uint32_t       PWM_TIM_CH;
	uint16_t       TIM_CLK_MHz;
	uint32_t       PWM_FREQ_Hz;
	uint8_t        PWM_RES_BITS;
}DC_MOTOR_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void DC_MOTOR_Init(uint8_t au8_MOTOR_Instance);
void DC_MOTOR_Start(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR, uint16_t au16_SPEED);
void DC_MOTOR_Set_Speed(uint8_t au8_MOTOR_Instance, uint16_t au16_SPEED);
void DC_MOTOR_Set_Dir(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR);
void DC_MOTOR_Stop(uint8_t au8_MOTOR_Instance);
uint32_t DC_MOTOR_Get_MaxFreq(uint8_t au8_MOTOR_Instance);


#endif /* DC_MOTOR_H_ */



/*
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, 0xffff);   //Move forward
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, 0xffff);
	HAL_Delay(100);

	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, 0xffff);  //Move backward
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, 0xffff);
	HAL_Delay(100);

	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, 0xffff);   //Move right
	DC_MOTOR_Stop(DC_MOTOR2);
	HAL_Delay(100);

	DC_MOTOR_Stop(DC_MOTOR1);                    //Move left
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, 0xffff);
	HAL_Delay(100);

	DC_MOTOR_Stop(DC_MOTOR1);                    //Stop
	DC_MOTOR_Stop(DC_MOTOR2);
	HAL_Delay(100);
*/
