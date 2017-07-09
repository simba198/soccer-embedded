/***** Includes *****/
#include "Dynamixel_AX-12A.h"
#include "stm32f4xx_hal_conf.h"

/********** Functions **********/
/********** Setters **********/
void Dynamixel_SetPosition(Dynamixel_HandleTypeDef *hdynamixel, double angle)
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

// UNIMPLEMENTED
void Dynamixel_SetVelocity(Dynamixel_HandleTypeDef *hdynamixel, double velocity){
	/* Sets the velocity of the motor */
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

// UNIMPLEMENTED
void Dynamixel_SetTorqueMaxMin(Dynamixel_HandleTypeDef *hdynamixel, uint8_t maxTorque, uint8_t minTorque);

// UNIMPLEMENTED
void Dynamixel_SetTorqueLimits(Dynamixel_HandleTypeDef *hdynamixel, uint8_t maxGoalTorque, uint8_t minGoalTorque);

// UNIMPLEMENTED
void Dynamixel_SetCWAngleLimit(Dynamixel_HandleTypeDef *hdynamixel, double maxAngle, double minAngle);

// UNIMPLEMENTED
void Dynamixel_SetCCWAngleLimit(Dynamixel_HandleTypeDef *hdynamixel, double maxAngle, double minAngle);

// UNIMPLEMENTED
void Dynamixel_SetCWComplianceMargin(Dynamixel_HandleTypeDef *hdynamixel, uint8_t CWcomplianceMargin);

// UNIMPLEMENTED
void Dynamixel_SetCCWComplianceMargin(Dynamixel_HandleTypeDef *hdynamixel, uint8_t CCWcomplianceMargin);

// UNIMPLEMENTED
void Dynamixel_SetCWComplianceSlope(Dynamixel_HandleTypeDef *hdynamixel, uint8_t CWcomplianceSlope);

// UNIMPLEMENTED
void Dynamixel_SetCCWComplianceMargin(Dynamixel_HandleTypeDef *hdynamixel, uint8_t CCWcomplianceSlope);

// UNIMPLEMENTED
void Dynamixel_TorqueEnable(Dynamixel_HandleTypeDef *hdynamixel, uint8_t isEnabled);


/********** GETTERS **********/
// UNIMPLEMENTED
void Dynamixel_GetPosition(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads addresses 0x24 and 0x25 in the motors RAM to see what the current position
	 * of the motor is.
	 * Writes the results to hdynamixel -> _lastPosition
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: none
	 */
}

// UNIMPLEMENTED
void Dynamixel_GetVelocity(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads addresses 0x26 and 0x27 in the motor RAM to see what the current velocity
	 * of the motor is.
	 * Writes the results to hdynamixel -> _lastVelocity
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: none
	 */
}

// UNIMPLEMENTED
void Dynamixel_GetTemperature(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads address 0x2B in the motor RAM to see what the current temperature is inside the motor.
	 * Writes the results to hdynamixel -> _lastTemperature
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: none
	 */
}

// UNIMPLEMENTED
void Dynamixel_GetLoad(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads addresses 0x28 and 0x29 in the motor RAM to see what the current
	 * load is.
	 * Writes the results to hdynamixel -> _lastLoad
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: none
	 */
}

// UNIMPLEMENTED
void Dynamixel_GetVoltage(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads address 0x2A in the motor RAM to see what the current voltage is.
	 * Writes the results to hdynamixel -> _lastVoltage
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: none
	 */
}

// UNIMPLEMENTED
uint8_t Dynamixel_IsMoving(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads the 0x2E address in motor RAM to see if motor is moving.
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: 0 if not moving
	 * 			1 if moving
	 */

	return 0;
}

// UNIMPLEMENTED
uint8_t Dynamixel_IsJointMode(Dynamixel_HandleTypeDef *hdynamixel){
	/* Reads the CW (addr: 0x06) and CCW (addr: 0x07) angle limits. If both are 0, motor is in wheel mode
	 * and can spin indefinitely. Otherwise, motor is in joint mode and has angle limits set
	 *
	 * Arguments: hdynamixel, the motor handle
	 *
	 * Returns: 0 if in wheel mode
	 * 			1 if in joint mode
	 */

	return 1;
}

/********** Computation **********/
uint8_t Dynamixel_ComputeChecksum(uint8_t *arr, int length){
	/* Compute the checksum for data to be transmitted */

	// Local variable declaration
	uint8_t accumulate = 0;

	// Loop through the array starting from the 2nd element of the array and finishing before the last
	// since the last is where the checksum will be stored
	for(uint8_t i = 2; i < length - 1; i++){
		accumulate += arr[i];
	}

	return 255 - (accumulate % 256); // Lower 8 bits of the logical NOT of the sum
}

/********** Initialization *********/
void Dynamixel_Init(Dynamixel_HandleTypeDef *hdynamixel, uint8_t ID,\
						   double BaudRate, UART_HandleTypeDef *UART_Handle)
{
	hdynamixel -> _ID = ID; // Motor ID (unique or global)
	hdynamixel -> _lastPosition = 0; // In future, initialize this accurately
	hdynamixel -> _lastVelocity = 0; // In future, initialize this accurately
	hdynamixel -> _lastTemperature = 0; // In future, initialize this accurately
	hdynamixel -> _lastVoltage = 0; // In future, initialize this accurately
	hdynamixel -> _lastLoad = 0; // In future, initialize this accurately
	hdynamixel -> _isJointMode = 1; // In future, initialize this accurately
	hdynamixel -> _UART_Handle = UART_Handle; // For UART TX and RX
}
