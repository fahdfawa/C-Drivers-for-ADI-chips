/*
 * ADXL_355.c
 *
 *  Created on: Oct 12, 2023
 *      Author: AN
 */





#include "ADXL355.h"

mxc_spi_req_t req;
uint8_t rx_data[DATA_LEN]={0};
uint8_t tx_data[DATA_LEN]={0};


int32_t volatile i32SensorX;
int32_t volatile i32SensorY;
int32_t volatile i32SensorZ;
int32_t volatile i32SensorT;
uint32_t volatile ui32SensorX;
uint32_t volatile ui32SensorY;
uint32_t volatile ui32SensorZ;
uint32_t volatile ui32SensorT;


void SPI_ReadWrite(uint8_t *tx_data, uint8_t *rx_data, uint8_t TotalNoOfBytes)
{
	// SPI Request to change register values
	req.spi = SPI;
	req.txData = (uint8_t*) tx_data;
	req.rxData = (uint8_t*) rx_data;
	req.txLen = TotalNoOfBytes;
	req.rxLen = TotalNoOfBytes;
	req.ssIdx = 0;
	req.txCnt = 0;
	req.rxCnt = 0;
	req.ssDeassert = 1; //Chip select to high after transaction

	MXC_SPI_MasterTransaction(&req); //Maxim inbuilt function for SPI Transfer

}

uint32_t ReadRegister(uint8_t RegAddress, uint8_t NoOfBytes)
{
	uint32_t ReturnData = 0;

	tx_data [0] = ADXL_READ | RegAddress<<1 ; //Adjust the command based on the ADXL read
	//SPI Read address given in command (1byte), the total SPI clock is command + Data read (size of the register)
	SPI_ReadWrite(tx_data, rx_data, (NoOfBytes + 1));

	//Loop to append the data
	//e.g. to read Filter register (3byte), ReturnData = rx_data[2]+rx_data[1]<<8)+rx_data[2]<<16
	for(int i=1;i<=NoOfBytes;i++)
	{
		ReturnData= ReturnData | rx_data[i]<<(8*(NoOfBytes-i));
	}

	return ReturnData;
}




/*@brief     Writing data to the given Register address
* @pre       tx_data[0] holds the command data and tx_data[1/2/3] holds data to be written to the register
* @param RegAddress RegisterAddress or use Macro defined in AD7124.h
* @param NoOfBytes Size of the Register address to be read 1/2/3 only
* @return none
*/
uint8_t WriteRegister(uint8_t address, uint32_t RegisterData, uint8_t NoOfBytes)
{
	//Command data - read/write
	//Loop to extract 8 bit register data for tx_data from the argument 'RegisterData'
	//e.g. tx_data[1]=RegisterData>>16; tx_data[2] = RegisterData>>8; tx_data[3]=RegisterData
	//lower-byte (8-bit) of RegisterData will be copied to tx_data[i]
	/*
	for(int i = 1; i<=NoOfBytes; i++)
	{
		tx_data[i] = RegisterData>>(8*(NoOfBytes-i));
	}
	*/
	//SPI write data - 'RegisterData' to address specified in tx_data[0]-command
	tx_data[0] = ADXL_WRITE | address<<1;
	tx_data[1] = RegisterData;
	SPI_ReadWrite(tx_data, rx_data, (NoOfBytes + 1));
	return *tx_data;
}


/**
   @brief Turns on accelerometer measurement mode.

   @return none

**/
void ADXL355_Start_Sensor(void)
{
   uint8_t ui8temp;
   ui8temp = ReadRegister(POWER_CTL,1);
   ui8temp = ui8temp & 0xFE;                              /* Set measurement bit in POWER_CTL register */
   printf("Power control Register: 0x%x\n", ui8temp);
   WriteRegister(POWER_CTL , 0x00, 1);                    /* Write the new value to POWER_CTL register */
}

/**
   @brief Puts the accelerometer into standby mode.
   @return none
**/
void ADXL355_Stop_Sensor(void)
{
	uint8_t ui8temp;
	ui8temp = ReadRegister(POWER_CTL,1);
	ui8temp = ui8temp & 0x01;                              /*Clear measurement bit in POWER_CTL register*/

	WriteRegister(POWER_CTL , 0x00, 1);                    /* Write the new value to POWER_CTL register */
}


void ADXL_Data_Scan(uint8_t address, uint8_t NoOfBytes)
{

	float AngleDegree_YZ = 0;
	float AngleDegree_ZX = 0;

	float Sensitivity = 3.9e-6;
	float ONEEIGHTY_BY_PI= 57.3029;

	ReadRegister(address, NoOfBytes);

	ui32SensorX = 0x000FFFFF&(((rx_data[1]<<16) | (rx_data[2]<<8) | rx_data[3])>>4);    // adjusting the empty bits and formating to 20bit data
	ui32SensorY = 0x000FFFFF&(((rx_data[4]<<16) | (rx_data[5]<<8) | rx_data[6])>>4);
	ui32SensorZ = 0x000FFFFF&(((rx_data[7]<<16) | (rx_data[8]<<8) | rx_data[9])>>4);

	i32SensorX = ADXL355_Data_Conversion(ui32SensorX);    //Converting to integer data
	i32SensorY = ADXL355_Data_Conversion(ui32SensorY);
	i32SensorZ = ADXL355_Data_Conversion(ui32SensorZ);

	float G_value_x = i32SensorX*Sensitivity;   //Converting raw data to "g" value
	float G_value_y = i32SensorY*Sensitivity;
	float G_value_z = i32SensorZ*Sensitivity;

	AngleDegree_YZ = atan(G_value_y/G_value_z);        //Angle in radians
	AngleDegree_YZ = AngleDegree_YZ * ONEEIGHTY_BY_PI;  //Angle in degrees
	AngleDegree_ZX = atan(G_value_x/G_value_z);
    AngleDegree_ZX = AngleDegree_ZX * ONEEIGHTY_BY_PI;

	MXC_Delay(300000);

    printf("Theta_YZ = %fDeg, Theta_XZ  %fDeg\n", AngleDegree_YZ, AngleDegree_ZX);

}

/**
   @brief Converting 2's complementary data to integer

   @return converted integer data
**/
int32_t ADXL355_Data_Conversion(uint32_t ui32SensorData)
{
	int32_t volatile i32Conversion = 0;

	if((ui32SensorData & 0x00080000) == 0x00080000)
	{
		i32Conversion = (ui32SensorData|0xFFF00000);
	}
	else
	{
		i32Conversion = ui32SensorData;
	}

	return i32Conversion;
}
