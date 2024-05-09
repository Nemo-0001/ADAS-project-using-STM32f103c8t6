/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DC_MOTOR1    0
#define DC_MOTOR2    1

#define HCSR04_SENSOR1  0
#define HCSR04_SENSOR2  1
#define HCSR04_SENSOR3  2

#define THRESHOLD_DISTANCE   20.0  //in centimeters

//#define RX_BUFFER_SIZE   20

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t TRIG_Ticks = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void Sys_Init(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  Sys_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /*Local variables begin*/
  float Front_Distance = 0.0, Right_Distance = 0.0, Left_Distance = 0.0;

  //char RX_data;
  /*Local variables end*/

  //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); // enabling UART Receiving

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*HAL_UART_Receive(&huart1 , &Rx_Data , 1, HAL_MAX_DELAY); // receiving the buffer

	  if(Rx_Data == 'f'){
	  		move_forward();
	  } else if(Rx_Data == 'r'){
	  		move_right();
	  } else if(Rx_Data == 'b'){
	  		move_backward();
	  } else if(Rx_Data == 'l'){
	  		move_left();
	  } else{
	  		stop();
	  }
	  HAL_Delay(500);*/

	Right_Distance = HCSR04_Read(HCSR04_SENSOR1);
    Front_Distance = HCSR04_Read(HCSR04_SENSOR2);
	Left_Distance = HCSR04_Read(HCSR04_SENSOR3);

	//convert_float_to_string(Front_Distance, (uint8_t*)MSG, sizeof(MSG));
	//HAL_UART_Transmit(&huart1, (uint8_t*)MSG, strlen((char*)MSG), HAL_MAX_DELAY);

	//sprintf(buffer, "%d\n", (int)Right_Distance);
	//HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

	/*HAL_UART_Receive(&huart1, (uint8_t *)RX_data, strlen(RX_data), HAL_MAX_DELAY);

	if((RX_data[strlen(RX_data) - 1] == '\n') || (strlen(RX_data) >= RX_BUFFER_SIZE - 1)){
		RX_data[strlen(RX_data)] = '\0'; //NULL-terminate the string

		if(strcmp(RX_data, "move_right") == 0){
			move_right();
		} else if(strcmp(RX_data, "move_left") == 0){
			move_left();
		} else if(strcmp(RX_data, "move_backward") == 0){
			move_backward();
		} else if(strcmp(RX_data, "stop") == 0){
			stop();
		} else{
			move_forward();
		}

		memset(RX_data, 0, sizeof(RX_data)); //Reset buffer
	}*/

	// Perform obstacle avoidance logic
	if (Right_Distance < THRESHOLD_DISTANCE || Front_Distance < THRESHOLD_DISTANCE || Left_Distance < THRESHOLD_DISTANCE) {
		// Obstacle detected
		// Choose appropriate avoidance maneuver
	    if (Right_Distance < Front_Distance && Right_Distance < Left_Distance) {
	    	// Obstacle is closer to sensor 1
	    	move_left();
	    } else if (Front_Distance < Right_Distance && Front_Distance < Left_Distance) {
	    	// Obstacle is closer to sensor 2
	    	stop();
	    	move_backward();
	    	HAL_Delay(50);

	    } else {
	    	// Obstacle is closer to sensor 3
	        move_right();
	    }
	} else {
		// No obstacle detected
	    move_forward();
	}
	HAL_Delay(100);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void Sys_Init(void)
{
	HCSR04_Init(HCSR04_SENSOR1, &htim4);
	HCSR04_Init(HCSR04_SENSOR2, &htim4);
	HCSR04_Init(HCSR04_SENSOR3, &htim3);
    DC_MOTOR_Init(DC_MOTOR1);
    DC_MOTOR_Init(DC_MOTOR2);
    DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, 0);
    DC_MOTOR_Start(DC_MOTOR2, DIR_CW, 0);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	HCSR04_TMR_IC_ISR(htim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	HCSR04_TMR_OVF_ISR(htim);
}

void SysTick_CallBack(void)
{
	TRIG_Ticks++;
    if(TRIG_Ticks >= 10) // Each 10msec
    {
    	HCSR04_Trigger(HCSR04_SENSOR1);
    	HCSR04_Trigger(HCSR04_SENSOR2);
    	HCSR04_Trigger(HCSR04_SENSOR3);
    	TRIG_Ticks = 0;
    }
}

void move_forward(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, 0xffff);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, 0xffff);
}


void move_backward(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, 0xffff);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, 0xffff);
}


void move_right(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, 0xffff);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, 0xffff);
}


void move_left(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, 0xffff);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, 0xffff);
}


void stop(void){
	DC_MOTOR_Stop(DC_MOTOR1);
	DC_MOTOR_Stop(DC_MOTOR2);
}

void convert_float_to_string(float value, const uint8_t *str, uint8_t array_size){
	snprintf((char*)str, array_size, "%.1f\r\n", value);
}

/*
// Function to turn the car in a random direction
void turn_random_direction(void) {
    // Generate a random number between 0 and 2
    int random_number = rand() % 3;

    switch(random_number) {
        case 0:
            // move forward (no turn)
            break;
        case 1:
            // Turn left
            move_left();
            break;
        case 2:
            // Turn right
            move_right();
            break;
        default:
            break;
    }
}

// Function to read distance from ultrasonic sensor at a specific angle
float ultrasonic_read_at_angle(int angle) {
    // Adjust angle to be within range [0, 360)
    angle = angle % 360;
    if (angle < 0) {
        angle += 360;
    }

    // Determine which ultrasonic sensor to use based on angle
    if (angle >= 315 || angle < 45) {
        // Front sensor
        return HCSR04_Read(HCSR04_SENSOR2);
    } else if (angle >= 45 && angle < 135) {
        // Right sensor
        return HCSR04_Read(HCSR04_SENSOR1);
    } else {
        // Left sensor
        return HCSR04_Read(HCSR04_SENSOR3);
    }
}*/


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
