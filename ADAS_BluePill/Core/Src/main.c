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
#define DC_MOTOR1       0
#define DC_MOTOR2       1

#define HCSR04_SENSOR1  0   //Right
#define HCSR04_SENSOR2  1   //Forward
#define HCSR04_SENSOR3  2   //Backward
#define HCSR04_SENSOR4  3   //Blind spot right distance
#define HCSR04_SENSOR5  4   //Blind spot left distance

#define GREEN_LED       0
#define YELLOW_LED      1
#define RED_LED         2

#define THRESHOLD_DISTANCE   20.0  //in centimeters
#define SAFE_DISTANCE        30.0
#define ADAPTIVE_DISTANCE    40.0

#define ALARM_OFF            0
#define ALARM_LOW_LEVEL      1
#define ALARM_MEDIUM_LEVEL   2
#define ALARM_HIGH_LEVEL     3

//Range of the speed is from : [0 ->> 65,535] | [0 ->> 0XFFFF]
#define MIN_SPEED            0
#define LOW_SPEED            0XFA
#define SPEED_1              0X1FF
#define SPEED_2              0XAFF
#define SPEED_3              0X1111
#define MAX_SPEED            0XFFFF

/* Macros for selecting the mode & the features of the car */
#define FORWARD                     'F'
#define BACKWARD                    'B'
#define RIGHT                       'R'
#define LEFT                        'L'
#define STOP                        'S'
#define OBSTACLE_AVOIDANCE          'O'
#define BLIND_SPOT_ADAPTIVE_CRUISE  'D'

//Speed levels
#define SPEED_LOW                   'Q'
#define SPEED1                      'W'
#define SPEED2            		    'X'
#define SPEED3                	    'Y'
#define SPEED_HIGH                  'U'

typedef enum {
	Forward = 0,
	Backward,
	Right,
	Left,
	Stop,
	Obstacle_Avoidance,
	BlindSpot_AdaptiveCruise
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

uint32_t Current_Time = 0;
float Speed = 0.0;

uint16_t current_speed = 0;
//uint16_t current_speed2 = 0;

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
  float Front_Distance = 0.0, Right_Distance = 0.0, Left_Distance = 0.0, BlindSpot_Right = 0.0, BlindSpot_Left = 0.0;

  /*Local variables end*/

  UART_SendString("UART is ready for sending & receiving...\r\n");
  HAL_Delay(50);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	switch (current_state) {
		case Forward:
			Front_Distance = HCSR04_Read(HCSR04_SENSOR2);
			if(Front_Distance > SAFE_DISTANCE){
				Alarm_Subsystem(ALARM_LOW_LEVEL);
			} else if((Front_Distance < SAFE_DISTANCE) && (Front_Distance > THRESHOLD_DISTANCE)){
				Alarm_Subsystem(ALARM_MEDIUM_LEVEL);
			} else{
				Alarm_Subsystem(ALARM_HIGH_LEVEL);
			}

			if (current_speed != 0) {
				UART_SendString("Moving forward with speed: ");
				UART_SendFloat(current_speed);
				UART_SendString("\r\n");
				move_forward(current_speed);
				HAL_Delay(300);
			} else {
				UART_SendString("Your speed is minimum\r\n");
				HAL_Delay(100);
			}
			current_state = Stop;
			break;

		case Backward:
			if (current_speed != 0) {
				UART_SendString("Moving backward with speed: ");
				UART_SendFloat(current_speed);
				UART_SendString("\r\n");
				move_backward(current_speed);
				HAL_Delay(300);
			} else {
				UART_SendString("Your speed is minimum\r\n");
				HAL_Delay(100);
			}
			current_state = Stop;
			break;

		case Right:
			Right_Distance = HCSR04_Read(HCSR04_SENSOR1);
			if (Right_Distance > SAFE_DISTANCE) {
				Alarm_Subsystem(ALARM_LOW_LEVEL);
			} else if ((Right_Distance < SAFE_DISTANCE) && (Right_Distance > THRESHOLD_DISTANCE)) {
				Alarm_Subsystem(ALARM_MEDIUM_LEVEL);
			} else {
				Alarm_Subsystem(ALARM_HIGH_LEVEL);
			}

			UART_SendString("Moving right\r\n");
			move_right(MAX_SPEED);
			HAL_Delay(300);
			current_state = Stop;
			break;

		case Left:
			Left_Distance = HCSR04_Read(HCSR04_SENSOR3);
			if (Left_Distance > SAFE_DISTANCE) {
				Alarm_Subsystem(ALARM_LOW_LEVEL);
			} else if ((Left_Distance < SAFE_DISTANCE) && (Left_Distance > THRESHOLD_DISTANCE)) {
				Alarm_Subsystem(ALARM_MEDIUM_LEVEL);
			} else {
				Alarm_Subsystem(ALARM_HIGH_LEVEL);
			}

			UART_SendString("Moving left\r\n");
			move_left(MAX_SPEED);
			HAL_Delay(300);
			current_state = Stop;
			break;

		case Stop:
			Alarm_Subsystem(ALARM_OFF);
			stop();
			UART_SendString("Stopped\r\n");
			break;

		case Obstacle_Avoidance:
			Right_Distance = HCSR04_Read(HCSR04_SENSOR1);

			Front_Distance = HCSR04_Read(HCSR04_SENSOR2);
			UART_SendString("Front distance: ");
			UART_SendFloat(Front_Distance);
			UART_SendString("\r\n");

			Left_Distance = HCSR04_Read(HCSR04_SENSOR3);

			Obdtacle_Avoidance(Front_Distance, Right_Distance, Left_Distance);
			HAL_Delay(50);

			break;

		case BlindSpot_AdaptiveCruise:
			Front_Distance = HCSR04_Read(HCSR04_SENSOR2);
			UART_SendString("Front distance: ");
			UART_SendFloat(Front_Distance);
			UART_SendString("\r\n");

			BlindSpot_Right = HCSR04_Read(HCSR04_SENSOR4);
			BlindSpot_Left = HCSR04_Read(HCSR04_SENSOR5);

			Adaptive_Cruise_Control(Front_Distance);
			HAL_Delay(50);

			BlindSpot_Detection(BlindSpot_Right, BlindSpot_Left);
			HAL_Delay(50);

			break;

			current_state = Stop;
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

/*------------------------- (Initialization) ---------------------------------*/
static void Sys_Init(void)
{
	HCSR04_Init(HCSR04_SENSOR1, &htim4);
	HCSR04_Init(HCSR04_SENSOR2, &htim4);
	HCSR04_Init(HCSR04_SENSOR3, &htim3);
	HCSR04_Init(HCSR04_SENSOR4, &htim3);
	HCSR04_Init(HCSR04_SENSOR5, &htim4);
    DC_MOTOR_Init(DC_MOTOR1);
    DC_MOTOR_Init(DC_MOTOR2);
    DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, MIN_SPEED);
    DC_MOTOR_Start(DC_MOTOR2, DIR_CW, MIN_SPEED);
    Buzzer_Init();
    LED_Init();
}
/*------------------------- (Initialization end) ---------------------------------*/


/*------------------------- (Interrupt handling start) ---------------------------------*/
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
    	HCSR04_Trigger(HCSR04_SENSOR5);
    	TRIG_Ticks = 0;
    }
}
/*------------------------- (Interrupt handling end) ---------------------------------*/



/*------------------------- (Movement control start) ---------------------------------*/
void move_forward(uint16_t SpeedF){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, SpeedF);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, SpeedF);
}


void move_backward(uint16_t SpeedB){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CW, SpeedB);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CW, SpeedB);
}


void move_right(uint16_t SpeedR){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, SpeedR);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, MIN_SPEED);
}


void move_left(uint16_t SpeedL){
	DC_MOTOR_Start(DC_MOTOR1, DIR_CCW, MIN_SPEED);
	DC_MOTOR_Start(DC_MOTOR2, DIR_CCW, SpeedL);
}


void stop(void){
	DC_MOTOR_Stop(DC_MOTOR1);
	DC_MOTOR_Stop(DC_MOTOR2);
}

void Speed_Control(uint16_t Speed1, uint16_t Speed2){
	DC_MOTOR_Set_Speed(DC_MOTOR1, Speed1);
	DC_MOTOR_Set_Speed(DC_MOTOR2, Speed2);
}

/*
 * @Objective: Calculate the speed in cm/s.
 *
 * */
float Calculate_Speed(float current_distance, uint32_t current_time){
    static float previous_distance = 0.0;
    static uint32_t previous_time = 0;
    float speed = 0.0;

    // Check if the previous time is not zero to avoid division by zero
    if (previous_time != 0) {
        // Calculate the time difference in seconds
        float time_diff = (current_time - previous_time) / 1000.0; // Assuming current_time is in milliseconds

        // Calculate the speed (distance change over time)
        speed = (current_distance - previous_distance) / time_diff; // Speed in cm/s
    }

    // Update the previous distance and time for the next calculation
    previous_distance = current_distance;
    previous_time = current_time;

    return (uint16_t)speed;
}
/*------------------------- (Movement control end) ---------------------------------*/



/*------------------------- (Features part start) ---------------------------------*/
void Obdtacle_Avoidance(float Front_Distance, float Right_Distance, float Left_Distance){
	// Perform obstacle avoidance logic
	if (Right_Distance < THRESHOLD_DISTANCE || Front_Distance < THRESHOLD_DISTANCE || Left_Distance < THRESHOLD_DISTANCE) {
		// Obstacle detected
		// Choose appropriate avoidance maneuver
		if (Right_Distance < Front_Distance && Right_Distance < Left_Distance) {
			// Obstacle is closer to sensor 1
 			UART_SendString("Moving left\r\n");
			move_left(current_speed);
		} else if (Front_Distance < Right_Distance && Front_Distance < Left_Distance) {
			// Obstacle is closer to sensor 2
			UART_SendString("Moving backward\r\n");
			stop();
			move_backward(current_speed);
			HAL_Delay(50);

			if (Right_Distance < Left_Distance) {
				UART_SendString("Moving left\r\n");
				move_left(current_speed);
				HAL_Delay(500);
				move_forward(current_speed);
			} else {
				UART_SendString("Moving right\r\n");
				move_right(current_speed);
				HAL_Delay(500);
				move_forward(current_speed);
			}
		} else {
			// Obstacle is closer to sensor 3
			UART_SendString("Moving right\r\n");
			move_right(current_speed);
		}
	} else {
		// No obstacle detected
		Current_Time = HAL_GetTick(); // Get the current time in milliseconds
		Speed = Calculate_Speed(Front_Distance, Current_Time);
		move_forward(current_speed);

		UART_SendString("Moving forward with speed: ");
		UART_SendFloat(Speed);
		UART_SendString("\r\n");
	}
}

void Adaptive_Cruise_Control(float Safe_Distance){
	//For safe distance detection
	if ((Safe_Distance < ADAPTIVE_DISTANCE) && (Safe_Distance > SAFE_DISTANCE)){
		Speed_Control(0X7FF, 0X7FF);
		UART_SendString("Speed has been limited to: ");
		UART_SendFloat(0X7FF);
		UART_SendString("\r\n");
	} else if((Safe_Distance < SAFE_DISTANCE) && (Safe_Distance > THRESHOLD_DISTANCE)){
		Speed_Control(0XFA, 0XFA);
		UART_SendString("Speed has been limited to: ");
		UART_SendFloat(0XFA);
		UART_SendString("\r\n");
	} else if (Safe_Distance < THRESHOLD_DISTANCE){
		stop();
		UART_SendString("Stopped\r\n");
	} else{
		Current_Time = HAL_GetTick(); // Get the current time in milliseconds
		Speed = Calculate_Speed(Safe_Distance, Current_Time);
		move_forward(current_speed);

		UART_SendString("Moving forward with speed: ");
		UART_SendFloat(Speed);
		UART_SendString("\r\n");
	}
}

void BlindSpot_Detection(float BlindSpot_Right, float BlindSpot_Left){
	//For blind spot detection
	if ((BlindSpot_Right > THRESHOLD_DISTANCE) && (BlindSpot_Right < SAFE_DISTANCE)){
		Alarm_Subsystem(ALARM_MEDIUM_LEVEL);
		UART_SendString("Take care of your right!! \r\n");
		UART_SendString("Your blind spot is not safe\r\n");
	} else if ((BlindSpot_Left > THRESHOLD_DISTANCE) && (BlindSpot_Left < SAFE_DISTANCE)){
		Alarm_Subsystem(ALARM_MEDIUM_LEVEL);
		UART_SendString("Take care of your left!! \r\n");
		UART_SendString("Your blind spot is not safe\r\n");
	} else if (BlindSpot_Right < THRESHOLD_DISTANCE){
		Alarm_Subsystem(ALARM_HIGH_LEVEL);
		UART_SendString("Danger on your right!!!\r\n");
	} else if (BlindSpot_Left < THRESHOLD_DISTANCE){
		Alarm_Subsystem(ALARM_HIGH_LEVEL);
		UART_SendString("Danger on your left!!!\r\n");
	} else{
		Alarm_Subsystem(ALARM_LOW_LEVEL);
	}
}

void Alarm_Subsystem(uint8_t alarmLevel) {
	if (ALARM_OFF == alarmLevel) {
		Buzzer_OFF();
		LED_OFF(GREEN_LED);
		LED_OFF(YELLOW_LED);
		LED_OFF(RED_LED);
	} else if (ALARM_LOW_LEVEL == alarmLevel) {
		Buzzer_OFF();
		LED_ON(GREEN_LED);
		LED_OFF(YELLOW_LED);
		LED_OFF(RED_LED);
	} else if (ALARM_MEDIUM_LEVEL == alarmLevel) {
		Buzzer_OFF();
		LED_OFF(GREEN_LED);
		LED_ON(YELLOW_LED);
		LED_OFF(RED_LED);
	} else if(ALARM_HIGH_LEVEL == alarmLevel){
		Buzzer_ON();
		LED_OFF(GREEN_LED);
		LED_OFF(YELLOW_LED);
		LED_ON(RED_LED);
	} else{
		/* Nothing */
	}
}
/*------------------------- (Features part end) ---------------------------------*/



/*------------------------- (Communication part start) ---------------------------------*/
/*
 * @Objective: Initiating receiving interrupt.
 *
 * */
void UART_Receiving_Init(void){
	 if (HAL_UART_Receive_IT(&huart1, &RX_Data, 1) != HAL_OK) {
		  Error_Handler();
	  }
}


//Function to send string over UART(Blocking mode)
void UART_SendString(char *string){
	uint16_t length = 0;
	while(string[length] != '\0'){
		length++;
	}

	HAL_UART_Transmit(&huart1, (uint8_t *)string, length, HAL_MAX_DELAY);
}

//Function to send float over UART(Blocking mode)
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

			case BLIND_SPOT_ADAPTIVE_CRUISE:
				current_state = BlindSpot_AdaptiveCruise;
				break;

			case SPEED_LOW:
				current_speed = LOW_SPEED;
				break;

			case SPEED1:
				current_speed = SPEED_1;
				break;

			case SPEED2:
				current_speed = SPEED_2;
				break;
			case SPEED3:
				current_speed = SPEED_3;
				break;

			case SPEED_HIGH:
				current_speed = MAX_SPEED;
				break;

			default:
				break;
		}
    }
}
/*------------------------- (Communication part end) ---------------------------------*/


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
