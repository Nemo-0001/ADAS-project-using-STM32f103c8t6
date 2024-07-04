# ADAS-project-using-STM32f103c8t6
This is my graduation project based on AI & Embedded systems.

Here's an example of a `README.md` file for your project:

---

# Car Obstacle Avoidance and Control System

This project implements an obstacle avoidance and control system for a car using an STM32 microcontroller. The system features multiple ultrasonic sensors for distance measurement, motor control, and UART communication.

## Features

- **Directional Control**: Move the car forward, backward, left, and right.
- **Obstacle Avoidance**: Detect obstacles and maneuver to avoid them.
- **Blind Spot Detection**: Monitor blind spots and alert if any object is detected.
- **Safe Distance Maintenance**: Maintain a safe distance from obstacles and control the speed accordingly.

## Peripherals

- **UART**: Used for communication and receiving commands.
- **Timers**: Used for handling ultrasonic sensor triggers and motor control.
- **Ultrasonic Sensors**: Used for distance measurement.
- **DC Motors**: Used for moving the car in different directions.
- **Buzzer**: Used for alerting in blind spot detection.

## Function Prototypes and Descriptions

### Initialization Functions

```c
static void Sys_Init(void);
```
Initializes the system peripherals including ultrasonic sensors, motors, and buzzer.

```c
void UART_Receiving_Init(void);
```
Initializes UART for receiving data.

### Movement Functions

```c
void move_forward(void);
```
Moves the car forward.

```c
void move_backward(void);
```
Moves the car backward.

```c
void move_right(void);
```
Moves the car to the right.

```c
void move_left(void);
```
Moves the car to the left.

```c
void stop(void);
```
Stops the car.

### Control Functions

```c
void Speed_Control(uint16_t Speed);
```
Controls the speed of the car.

### Logic Functions

```c
void ObdtacleAvoidance_Logic(float Front_Distance, float Right_Distance, float Left_Distance);
```
Performs obstacle avoidance logic based on distance measurements from the sensors.

```c
void SafeDistance_Logic(float Safe_Distance);
```
Maintains a safe distance from obstacles and controls the speed accordingly.

```c
void BlindSpot_Logic(float BlindSpot_right, float BlindSpot_left);
```
Monitors blind spots and triggers the buzzer if an object is detected.

### UART Communication Functions

```c
void UART_SendString(char *string);
```
Sends a string over UART.

```c
void UART_SendFloat(float num);
```
Sends a float value over UART as a string.

### Callback Functions

```c
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
```
Handles input capture interrupts for ultrasonic sensors.

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
```
Handles timer period elapsed interrupts for ultrasonic sensors.

```c
void SysTick_CallBack(void);
```
Handles SysTick interrupts for periodic ultrasonic sensor triggering.

```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
```
Handles UART receive complete interrupts and processes incoming commands.

## Usage

1. Clone the repository to your local machine.
2. Open the project in your preferred IDE (e.g., STM32CubeIDE).
3. Connect the necessary peripherals as described above.
4. Build and flash the project to your STM32 microcontroller.
5. Use a UART terminal to send commands to the car:
   - 'F': Move forward
   - 'B': Move backward
   - 'R': Move right
   - 'L': Move left
   - 'S': Stop
   - 'O': Enable obstacle avoidance
   - 'D': Enable blind spot safe distance monitoring.
6. Try the code on the simulation in Simulation directory

## Simulation

You can see here the simulation and if you pressed the img you can find the video of the simulation on my channel on YouTube.

[![Watch the simulation](Simulation/Simulation.JPG)](https://youtu.be/wQjUKuoTerA?si=SQomSVhpP3Q2sc1S)

## License

This project is licensed under the MIT License.





