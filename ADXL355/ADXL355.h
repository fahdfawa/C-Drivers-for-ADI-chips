/*
 * ADXL355.h
 *
 *  Created on: Aug 16, 2022
 *      Author: SSamkutt
 */

#ifndef ADXL355_H_
#define ADXL355_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "nvic_table.h"
#include "uart.h"
#include "spi.h"
#include "dma.h"
#include "gpio.h"
#include "board.h"
#include "pb.h"




/***** Definitions *****/
#define DATA_LEN        100         // Words
#define DATA_VALUE      0xA5A5      // This is for master mode only...
#define VALUE           0xFFFF
#define SPI_SPEED       200000      // Bit Rate






#define SPI         MXC_SPI0
#define SPI_IRQ     SPI0_IRQn

/* ADXL355 registers addresses */
#define ADXL_READ				 0x01
#define ADXL_WRITE				 0x00
#define DEVID_AD                 0x00
#define DEVID_MST                0x01
#define PARTID                   0x02
#define REVID                    0x03
#define STATUS                   0x04

#define XDATA3                   0x08
#define XDATA2                   0x09
#define XDATA1                   0x0A
#define YDATA3                   0x0B
#define YDATA2                   0x0C
#define YDATA1                   0x0D
#define ZDATA3                   0x0E
#define ZDATA2                   0x0F
#define ZDATA1                   0x10

#define POWER_CTL                0x2D
#define SELF_TEST                0x2E
#define RESET                    0x2F


void SPI_ReadWrite(uint8_t *tx_data, uint8_t *rx_data, uint8_t TotalNoOfBytes);
uint32_t ReadRegister(uint8_t RegAddress, uint8_t NoOfBytes);
void ADXL355_Start_Sensor(void);
void ADXL355_Stop_Sensor(void);
void ADXL_Data_Scan(uint8_t address, uint8_t NoOfBytes);
int32_t ADXL355_Data_Conversion(uint32_t ui32SensorData);


#endif /* ADXL355_H_ */
