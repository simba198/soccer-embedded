/***** Drivers for Dynamixel AX-12A Servo Motor *****/

/***** Define to prevent recursive inclusion *****/
#ifndef __DYNAMIXEL_AX_12A_H
#define __DYNAMIXEL_AX_12A_H

/***** Includes *****/
#include "stm32f4xx_hal.h"

/***** Macros *****/
#define __DYNAMIXEL_TRANSMIT() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 1)
#define __DYNAMIXEL_RECEIVE() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, 1)

/***** Private variables *****/
UART_HandleTypeDef Dynamixel_huart;

/***** Exported types *****/
typedef struct{
	uint8_t					_ID;					/*!< Motor identification (0-252)					*/
	uint8_t					_BaudRate;				/*!< UART communication baud rate motor uses		*/
	uint16_t				_curPosition;			/*!< Position read from motor						*/
	uint16_t				_curVelocity;			/*!< Velocity read from motor						*/
	UART_HandleTypeDef		*_UART_Handle;			/*!< UART handle for motor							*/
}Dynamixel_HandleTypeDef;

/***** Private function prototypes *****/
void Dynamixel_Init(Dynamixel_HandleTypeDef *hdynamixel, uint8_t ID, double BaudRate, UART_HandleTypeDef* UART_Handle);
void Dynamixel_SetAngle(Dynamixel_HandleTypeDef *hdynamixel, double angle);
void Dynamixel_SetBaudRate(Dynamixel_HandleTypeDef *hdynamixel, double baud);
void Dynamixel_SetID(Dynamixel_HandleTypeDef *hdynamixel, int ID);
uint8_t Dynamixel_ComputeChecksum(uint8_t *arr, int length);

#endif /* __DYNAMIXEL_AX-12A_H */