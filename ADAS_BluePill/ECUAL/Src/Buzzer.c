/*
 * Buzzer.c
 *
 *  Created on: Jun 27, 2024
 *      Author: Abdallah Alnemr
 */

#include "../Inc/Buzzer.h"

void Buzzer_Init(void){
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
}

void Buzzer_ON(void){
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
}

void Buzzer_OFF(void){
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
}




