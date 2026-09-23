#include "main.h"

#define OFFSET_X_REG_L 0x45
#define OFFSET_X_REG_H 0x46
#define OFFSET_Y_REG_L 0x47
#define OFFSET_Y_REG_H 0x48
#define OFFSET_Z_REG_L 0x49
#define OFFSET_Z_REG_H 0x4A

#define WHO_AM_I 0x4F

#define CFG_REG_A 0x60
#define CFG_REG_B 0x61
#define CFG_REG_C 0x62

#define INT_CRTL_REG 0x63
#define INT_SOURCE_REG 0x64
#define INT_THS_L_REG 0x65
#define INT_THS_H_REG 0x66

#define STATUS_REG 0x67

#define OUTX_L_REG 0x68
#define OUTX_H_REG 0x69
#define OUTY_L_REG 0x6A
#define OUTY_H_REG 0x6B
#define OUTZ_L_REG 0x6C
#define OUTZ_H_REG 0x6D

#define TEMP_OUT_L_REG 0x6E
#define TEMP_OUT_H_REG 0x6F

#define LIS2MDL_CS_PORT GPIOB
#define LIS2MDL_CS_PIN  GPIO_PIN_6

#define LIS2MDL_SPI_READ 0x80
#define LIS2MDL_SPI_WRITE 0x7F

extern SPI_HandleTypeDef hspi2;

int LIS2MDL_Init();
int LIS2MDL_Read_mgauss(float xyz[3]);
