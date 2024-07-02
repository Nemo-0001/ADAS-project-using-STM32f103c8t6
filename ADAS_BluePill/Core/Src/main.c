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
extern UART_HandleTypeDef huart1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DC_MOTOR1    0
#define DC_MOTOR2    1

#define HCSR04_SENSOR1  0   //Right
#define HCSR04_SENSOR2  1   //Forward
#define HCSR04_SENSOR3  2   //Backward
#define HCSR04_SENSOR4  3   //For safe distance*/

#define THRESHOLD_DISTANCE   15.0  //in centimeters
#define SAFE_DISTANCE        20.0
#define BLINDSPOT_DISTANCE   25.0

//Range of the speed is from : [0 ->> 65,535] | [0 ->> 0XFFFF]
#define MIN_SPEED            0
#define MAX_SPEED            0XFFFF

/* Macros for selecting the mode & the features of the car */
#define FORWARD                     'F'
#define BACKWARD                    'B'
#define RIGHT                       'R'
#define LEFT                        'L'
#define STOP                        'S'
#define OBSTACLE_AVOIDANCE          'O'
#define BLIND_SPOT_SAFE_DISTANCE    'D'

typedef enum {
	Forward = 0,
	Backward,
	Right,
	Left,
	Stop,
	Obstacle_Avoidance,
	Blind_Spot
} state;

state current_state = Stop;
//#define RX_BUFFER_SIZE   20

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t TRIG_Ticks = 0;
uint8_t RX_Data = 0;
volatile uint8_t new_data_flag = 0;

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

  UART_Receiving_Init();

  /*Local variables begin*/
  float Front_Distance = 0.0, Right_Distance = 0.0, Left_Distance = 0.0, Safe_Distance = 0.0;
  /*Local variables end*/

  UART_SendString("UART is ready for sending & receiving...\r\n");
  HAL_Delay(100);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	switch (current_state) {
		case Forward:
			UART_SendString("Moving forward\n");
			move_forward();
			HAL_Delay(500);
			current_state = Stop;
			break;

		case Backward:
			UART_SendString("Moving backward\n");
			move_backward();
			HAL_Delay(500);
			current_state = Stop;
			break;

		case Right:
			UART_SendString("Moving right\n");
			move_right();
			HAL_Delay(500);
			current_state = Stop;
			break;

		case Left:
			UART_SendString("Moving left\n");
			move_left();
			HAL_Delay(500);
			current_state = Stop;
			break;

		case Stop:
			stop();
			UART_SendString("Stopped\n");
			break;

		case Obstacle_Avoidance:
			Right_Distance = HCSR04_Read(HCSR04_SENSOR1);

			Front_Distance = HCSR04_Read(HCSR04_SENSOR2);
			UART_SendString("Front Distance: ");
			UART_SendFloat(Front_Distance);
			UART_SendString("\n");

			Left_Distance = HCSR04_Read(HCSR04_SENSOR3);

			ObdtacleAvoidance_Logic(Front_Distance, Right_Distance, Left_Distance);
			HAL_Delay(10);
			break;

		case Blind_Spot:
			Right_Distance = HCSR04_Read(HCSR04_SENSOR1);
			Left_Distance = HCSR04_Read(HCSR04_SENSOR3);
			Safe_Distance = HCSR04_Read(HCSR04_SENSOR4);
			UART_SendString("Front Distance: ");
			UART_SendFloat(Safe_Distance);
			UART_SendString("\n");

			SafeDistance_Logic(Safe_Distance);
			HAL_Delay(50);

			BlindSpot_Logic(Right_Distance, Left_Distance);
			HAL_Delay(50);

			break;

		default:
			break;
	}

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
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
	HCSR04_Init(HCSR04_SENSOR4, &htim3);
    DC_MOTOR_Init(DC_MOTOR1);
    DC_MOTOR_Init(DC_MOTOR2);
    DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, MIN_SPEED);
    DC_MOTOR_Start(DC_MOTOR2, DIR_CW, MIN_SPEED);
    Buzzer_Init();
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
    	HCSR04_Trigger(HCSR04_SENSOR4);
    	TRIG_Ticks = 0;
    }
}

void move_forward(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, MAX_SPEED);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, MAX_SPEED);
}


void move_backward(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, MAX_SPEED);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, MAX_SPEED);
}


void move_right(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, MAX_SPEED);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, MAX_SPEED);
}


void move_left(void){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, MAX_SPEED);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, MAX_SPEED);
}


void stop(void){
	DC_MOTOR_Stop(DC_MOTOR1);
	DC_MOTOR_Stop(DC_MOTOR2);
}

void Speed_Control(uint16_t Speed){
	DC_MOTOR_Set_Speed(DC_MOTOR1, Speed);
	DC_MOTOR_Set_Speed(DC_MOTOR2, Speed);
}

void UART_Receiving_Init(void){
	 if (HAL_UART_Receive_IT(&huart1, &RX_Data, 1) != HAL_OK) {
		  Error_Handler();
	  }
}

void ObdtacleAvoidance_Logic(float Front_Distance, float Right_Distance, float Left_Distance){
	// Perform obstacle avoidance logic
	if (Right_Distance < THRESHOLD_DISTANCE || Front_Distance < THRESHOLD_DISTANCE || Left_Distance < THRESHOLD_DISTANCE) {
		// Obstacle detected
		// Choose appropriate avoidance maneuver
		if (Right_Distance < Front_Distance && Right_Distance < Left_Distance) {
			// Obstacle is closer to sensor 1
//			UART_SendString("Object detected on the right\n");
//			UART_SendString("Moving left\n");
			move_left();
		} else if (Front_Distance < Right_Distance && Front_Distance < Left_Distance) {
			// Obstacle is closer to sensor 2
//			UART_SendString("Object detected on the forward\n");
//			UART_SendString("Moving backward\n");
			stop();
			move_backward();
			HAL_Delay(50);

			if (Right_Distance < Left_Distance) {
//				UART_SendString("Object detected on the right\n");
//				UART_SendString("Moving left\n");
				move_left();
				HAL_Delay(500);
				move_forward();
			} else {
//				UART_SendString("Object detected on the left\n");
//				UART_SendString("Moving right\n");
				move_right();
				HAL_Delay(500);
				move_forward();
			}
		} else {
			// Obstacle is closer to sensor 3
//			UART_SendString("Object detected on the left\n");
//			UART_SendString("Moving right\n");
			move_right();
		}
	} else {
		// No obstacle detected
//		UART_SendString("No objects detected\n");
//		UART_SendString("Moving forward\n");
//		HAL_Delay(10);
		move_forward();
	}
}

void SafeDistance_Logic(float Safe_Distance){
	//For safe distance detection
	if(Safe_Distance < SAFE_DISTANCE){
		Speed_Control(0XFFF);
		/*UART_SendString("Object detected!!\n");
		UART_SendString("Speed has been decreased\n");*/
	} else if (Safe_Distance < THRESHOLD_DISTANCE) {
		stop();
		/*UART_SendString("Object detected!!\n");
		UART_SendString("Stopped\n");*/
	} else {
		move_backward();
		//UART_SendString("Moving forward\n");
	}
}

void BlindSpot_Logic(float Right_Distance, float Left_Distance){
	//For blind spot detection
	if (Right_Distance < BLINDSPOT_DISTANCE || Left_Distance < BLINDSPOT_DISTANCE) {
		Buzzer_ON();
		//ART_SendString("Be careful!! Your blind spot is not safe\n");
	} else {
		Buzzer_OFF();
		//UART_SendString("Your blind spot is safe\n");
	}
}

//Function to send string over UART
void UART_SendString(char *string){
	uint16_t length = 0;
	while(string[length] != '\0'){
		length++;
	}

	HAL_UART_Transmit(&huart1, (uint8_t *)string, length, HAL_MAX_DELAY);
}

void UART_SendFloat(float num){
    char buffer[20];               // Buffer to hold the string representation of the float
    sprintf(buffer, "%.2f", num);  // Convert float to string with 2 decimal places
    UART_SendString(buffer);       // Send the string via UART
}

//Auto Called when receiving a new data over UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == USART1) {
    	//new_data_flag = 1;
        HAL_UART_Receive_IT(&huart1, &RX_Data, 1);
		switch (RX_Data) {
			case FORWARD:
				current_state = Forward;
				break;

			case BACKWARD:
				current_state = Backward;
				break;

			case RIGHT:
				current_state = Right;
				break;

			case LEFT:
				current_state = Left;
				break;

			case STOP:
				current_state = Stop;
				break;

			case OBSTACLE_AVOIDANCE:
				current_state = Obstacle_Avoidance;
				break;

			case BLIND_SPOT_SAFE_DISTANCE:
				current_state = Blind_Spot;
				break;

			default:
				break;
		}

    }
}

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
