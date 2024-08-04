/******************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/***** Includes *****/



#include "ADXL355.h"

/***** Definitions *****/

/***** Globals *****/





/***** Functions *****/

// *****************************************************************************
int main(void)
{
	uint8_t Reg_Data = 0;

    //SPI Configuration - Speed, Polarity, Phase, Data size
    MXC_SPI_Init(SPI, 1, 0, 1, 0, SPI_SPEED);
    MXC_SPI_SetDataSize(SPI, 8);
    MXC_SPI_SetMode(SPI, SPI_MODE_0);
    memset(rx_data, 0x0, DATA_LEN * sizeof(uint8_t));

    Reg_Data = ReadRegister(DEVID_AD, 1);
    printf("DEVID_AD: 0x%x\n", Reg_Data);
    printf("tx_data = %X \n",tx_data);
    Reg_Data = ReadRegister(DEVID_MST, 1);
    printf("DEVID_MST: 0x%x\n", Reg_Data);
    Reg_Data = ReadRegister(PARTID, 1);
    printf("PARTID: 0x%x\n", Reg_Data);
    Reg_Data = ReadRegister(REVID, 1);
    printf("REVID: 0x%x\n", Reg_Data);
    Reg_Data = ReadRegister(STATUS, 1);
    printf("Status before enabling the sensor : 0x%x\n", Reg_Data);

    ADXL355_Start_Sensor();
    Reg_Data = ReadRegister(STATUS, 1);
    printf("Status after enabling the sensor : 0x%x\n", Reg_Data);
    Reg_Data = ReadRegister(0x2C, 1);
    printf("Range Selected before : 0x%x\n", Reg_Data);
   // WriteRegister(0x2C, 0x01, 1);
    Reg_Data = ReadRegister(0x2C, 1);
    printf("Range Selected after: 0x%x\n", Reg_Data);

    for(int count=0; count<=10000; count++)
    {

    	Reg_Data = ReadRegister(STATUS, 1);
    	if((Reg_Data&0x01)== 0x01)
    	{

    		ADXL_Data_Scan(XDATA3, 9); //start address and total register to read
    	}

    }
    ADXL355_Stop_Sensor();
    printf("Reading over\n");

}

