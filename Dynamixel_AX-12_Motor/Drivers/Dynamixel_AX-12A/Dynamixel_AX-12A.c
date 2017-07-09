/***** Includes *****/
#include "Dynamixel_AX-12A.h"
#include "stm32f4xx_hal_conf.h"

/***** Functions *****/
void Dynamixel_SetAngle(Dynamixel_HandleTypeDef *hdynamixel, double angle)
{
	/* Takes a double between 0 and 300, encodes this position in an
	 * upper and low hex byte pair (with a maximum of 1023 as defined in the AX-12
	 * user manual), and sends this information (along with requisites) over UART
	 */

	// Definition array to be transmitted
	uint8_t arrTransmit[9];

	// Do assignments and computations
	arrTransmit[0] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[1] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[2] = hdynamixel -> _ID; // Motor ID
	arrTransmit[3] = 0x05; // Length of message minus the obligatory bytes
	arrTransmit[4] = 0x03; // WRITE instruction
	arrTransmit[5] = 0x1e; // Address of goal position

	// Translate the angle from degrees into a 10-bit number
	int normalized_value = (int)(angle / 300 * 1023); // maximum angle of 1023
	arrTransmit[6] = normalized_value & 0xFF; // Low byte of goal position
	arrTransmit[7] = (normalized_value >> 8) & 0xFF; // High byte of goal position

	// Checksum = 255 - (sum % 256)
	arrTransmit[8] = Dynamixel_ComputeChecksum(arrTransmit, 9);

	// Set data direction
	__DYNAMIXEL_TRANSMIT();

	// Transmit
	HAL_UART_Transmit(hdynamixel -> _UART_Handle, arrTransmit, 9, 100);
}

void Dynamixel_SetBaudRate(Dynamixel_HandleTypeDef *hdynamixel, double baud){
	/* Set baud rate of a particular motor */

	// Define array for transmission
	uint8_t arrTransmit[8];

	// Do assignments and computations
	arrTransmit[0] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[1] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[2] = hdynamixel -> _ID; // Motor ID
	arrTransmit[3] = 0x04; // Length of message minus the obligatory bytes
	arrTransmit[4] = 0x03; // WRITE instruction
	arrTransmit[5] = 0x04; // Baud rate register address

	// Set _baud equal to the hex code corresponding to baud. Default to 1 Mbps
	if(baud > 0){
		// Valid for baud in range [1000000, 7844]. Will be converted to 8-bit resolution
		hdynamixel -> _BaudRate = (uint8_t)((2000000 / baud) - 1);
	}
	else{
		// Default to 1 Mbps
		hdynamixel -> _BaudRate = 0x01;
	}
	arrTransmit[6] = hdynamixel -> _BaudRate; // The hex code corresponding to the desired baud rate

	// Checksum = 255 - (sum % 256)
	arrTransmit[7] = Dynamixel_ComputeChecksum(arrTransmit, 8);

	// Set data direction
	__DYNAMIXEL_TRANSMIT();

	// Transmit
	HAL_UART_Transmit(hdynamixel -> _UART_Handle, arrTransmit, 8, 100);
}

void Dynamixel_SetID(Dynamixel_HandleTypeDef *hdynamixel, int ID){
	/* Set motor ID */

	// Define array for transmission
	uint8_t arrTransmit[8]; // Obligatory bytes for setting baud rate

	// Do assignments and computations
	arrTransmit[0] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[1] = 0xff; // Obligatory bytes for starting communication
	arrTransmit[2] = hdynamixel -> _ID; // Motor ID
	arrTransmit[3] = 0x04;
	arrTransmit[4] = 0x03;
	arrTransmit[5] = 0x03; // ID register address
	arrTransmit[6] = ID; // ID to be assigned

	// Checksum = 255 - (sum % 256)
	arrTransmit[7] = Dynamixel_ComputeChecksum(arrTransmit, 8);

	// Set data direction
	__DYNAMIXEL_TRANSMIT();

	// Transmit
	HAL_UART_Transmit(hdynamixel -> _UART_Handle, arrTransmit, 8, 100);
}

uint8_t Dynamixel_ComputeChecksum(uint8_t *arr, int length){
	/* Compute the checksum for data to be transmitted */

	// Local variable declaration
	uint8_t accumulate = 0;

	// Loop through the array starting from the 2nd element of the array and finishing before the last
	// since the last is where the checksum will be stored
	for(uint8_t i = 2; i++; i < length - 1){
		accumulate += arr[i];
	}

	return 255 - (accumulate % 256); // Lower 8 bits of the logical NOT of the sum
}

void Dynamixel_Init(Dynamixel_HandleTypeDef *hdynamixel, uint8_t ID,\
						   double BaudRate, UART_HandleTypeDef* UART_Handle)
{
	hdynamixel -> _ID = ID; // Motor ID (unique or global)
	Dynamixel_SetBaudRate(hdynamixel, BaudRate); // UART communication baud rate
	hdynamixel -> _curPosition = 0; // In future, initialize this accurately
	hdynamixel -> _curVelocity = 0; // In future, initialize this accurately
	hdynamixel -> _UART_Handle = UART_Handle; // For UART TX and RX
}