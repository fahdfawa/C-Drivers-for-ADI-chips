////////////////Created by Fahad Ahammad////////////////////////
///////////////CAP APR//////////////////////////////////////


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

