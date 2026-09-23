#include "lis2mdl.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include <stdint.h>

static inline void LIS2MDL_CS_LOW(void) {
	HAL_GPIO_WritePin(LIS2MDL_CS_PORT, LIS2MDL_CS_PIN, GPIO_PIN_RESET);
}

static inline void LIS2MDL_CS_HIGH(void) {
	HAL_GPIO_WritePin(LIS2MDL_CS_PORT, LIS2MDL_CS_PIN, GPIO_PIN_SET);
}

HAL_StatusTypeDef LIS2MDL_Read(uint8_t reg, uint8_t* buf, uint16_t n) {
	// Read command
	uint8_t cmd = reg | 0x80;
	LIS2MDL_CS_LOW();
	HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi2, &cmd, 1, 10);
	if (status == HAL_OK)
		status = HAL_SPI_Receive(&hspi2, buf, n, 10);
	LIS2MDL_CS_HIGH();
	return status;
}

HAL_StatusTypeDef LIS2MDL_Write(uint8_t reg, uint8_t val) {
	// Write command and value
	uint8_t tx[2] = {reg & 0x7F, val };
	LIS2MDL_CS_LOW();
	HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi2, tx, 1, 10);
	LIS2MDL_CS_HIGH();
	return status;
}

int LIS2MDL_Init() {
	LIS2MDL_CS_HIGH();
	HAL_Delay(20);

	// Soft reset and then disable i2c, has error correction and enables spi on pin 7
	LIS2MDL_Write(CFG_REG_A, 0x20); // SOFT_RST
	HAL_Delay(5);
	LIS2MDL_Write(CFG_REG_C, 0x34); // I2C_DIS | BDU | 4WSPI
	
	// Makes sure correct who am i is returned. if incorrect this is the wrong chip
	uint8_t id = 0;
	if (LIS2MDL_Read(WHO_AM_I, &id, 1) != HAL_OK || id != 0x40) return -1;

	// Enables temperature compensation, 100 Hz output data rate, Continous mode (data will be placed in register constantly)
	LIS2MDL_Write(CFG_REG_A, 0x8C); // COMP_TEMP_EN | ODR1 | ODR1
}

// Read data in milligauss
int LIS2MDL_Read_mgauss(float xyz[3]) {
	// Read all output registers into the 6 bytes
	uint8_t b[6];
	if (LIS2MDL_Read(OUTX_L_REG, b, 6) != HAL_OK) return -1;
	// convert 16 bit numbers to floats
	for (int i = 0; i < 3; i++)
		xyz[i] = (int16_t)(b[2*i] | (b[2*i+1] << 8)) * 1.5f;   // mG
	return 0;
}
