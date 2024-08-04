
/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2c.h"
#include "dma.h"

#include "MAX20360_fields.h"
#include "MAX20360_functions.h"
#include "MAX20360_Registers.h"


// #define MASTERDMA


/***** Globals *****/


volatile uint8_t DMA_FLAG = 0;
volatile int I2C_FLAG;
volatile int txnum = 0;
volatile int txcnt = 0;
volatile int rxnum = 0;
volatile int num;

/***** Functions *****/



// *****************************************************************************
int main()
{

	//I2C Initialization

    MXC_I2C_Init(I2C_MASTER, 1, 0);
    NVIC_EnableIRQ(I2C0_IRQn);
    MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ);


    //Reading and verifying chipID

    uint8_t chipID[1];
    Register_Read(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, CHIPID, chipID);
	uint8_t chipid = chipID[0];
	printf("ChipID = %x\n",chipid);

	//Declaring instances for unions (registers)

    //instance for LDO1
	Combined_bit_fields_Enable_Config     combined_LDO_1_en;
	Combined_bit_fields_LDO_1_conf        combined_LDO_1_conf;

	//instance for LDO2
	Combined_bit_fields_Enable_Config     combined_LDO_2_en;
    Combined_bit_fields_LDO_2_conf        combined_LDO_2_conf;

	//instance for Buck1
    Combined_bit_fields_Enable_Config     combined_Buck_1_en;
    Combined_bit_fields_Buck_Cfg0         combined_Buck_1_conf0;
    Combined_bit_fields_Buck_Cfg1         combined_Buck_1_conf1;

    //instance for Buck2
    Combined_bit_fields_Enable_Config     combined_Buck_2_en;
    Combined_bit_fields_Buck_Cfg0         combined_Buck_2_conf0;
    Combined_bit_fields_Buck_Cfg1         combined_Buck_2_conf1;

    //instance for Buck3
	Combined_bit_fields_Enable_Config     combined_Buck_3_en;
	Combined_bit_fields_Buck_Cfg0         combined_Buck_3_conf0;
	Combined_bit_fields_Buck_Cfg1         combined_Buck_3_conf1;

    //instance for Boost
	Combined_bit_fields_Enable_Config     combined_Boost_en;
	Combined_bit_fields_Boost_Cfg         combined_Boost_conf;

    //instance for Buck-Boost
	Combined_bit_fields_Enable_Config     combined_BBst_en;
	Combined_bit_fields_BBst_Cfg0         combined_BBst_conf0;
	Combined_bit_fields_BBst_Cfg1         combined_BBst_conf1;

    //Combined_bit_fields_LDO_ctrl combined_LDO_2_ctrl;  //un-comment if using MPC control
    //use this function to lock the converter voltage
    Lock(LOCK_UNLOCK_ALL, UNLOCK); //for locking/unlocking the LDO2




    //LDO_configure(LDO_2, DISABLED, &combined_LDO_1_en, &combined_LDO_1_conf, &combined_LDO_2_conf);         //configuring LDO_2
    LDO_2_Set_Voltage(1400); //Expected value in mV range (900mV to 4000mV) in steps of 100mV
    LDO_configure(LDO_2, ENABLED, &combined_LDO_1_en, &combined_LDO_1_conf, &combined_LDO_2_conf);         //configuring LDO_2

    //Note: First Enable Buck1 then LDO1 since LDO1 input is connected to Buck1 output

    Buck1_configure(ENABLED, &combined_Buck_1_en, &combined_Buck_1_conf0, &combined_Buck_1_conf1);   //Buck1 can't be diabled :Bk1step = 50mV in MAX20360J version
    Buck1_Set_Voltage(2700); //Expected value in mV range (550mV to 3150mV) in steps of 50mV

    Buck2_configure(ENABLED, &combined_Buck_2_en, &combined_Buck_2_conf0, &combined_Buck_2_conf1);
    Buck2_Set_Voltage(2400); //Expected value in mV range (550mV to 3150mV) in steps of 50mV

    Buck3_configure(ENABLED, &combined_Buck_3_en, &combined_Buck_3_conf0, &combined_Buck_3_conf1);  //Buck3 can't be disbked :Bkstep = 50mV in MAX20360J version
    Buck3_Set_Voltage(3700);

    //Note: Buck1 output is connected to input of LDO1. Sequence of LDO1 config:  Buck1 en-> set buck1 voltage-> LDO1 disable-> set LDO1 voltage -> LDO1 enable
    LDO_configure(LDO_1, DISABLED, &combined_LDO_2_en, &combined_LDO_1_conf, &combined_LDO_2_conf);
    LDO_1_Set_Voltage(1000); //Expected value in mV range (500mV to 1950mV) in steps of 25mV
    LDO_configure(LDO_1, ENABLED, &combined_LDO_2_en, &combined_LDO_1_conf, &combined_LDO_2_conf);

    BB_configure(ENABLED, &combined_BBst_en, &combined_BBst_conf0, &combined_BBst_conf1);
    BB_Set_Voltage(3200); //Expected value in mV range (2500mV to 5500mV) in steps of 50mV

    Boost_configure(ENABLED, &combined_Boost_en, &combined_Boost_conf);
    Boost_Set_Voltage(15000); //Expected value in mV range (5000mV to 20000mV) in steps of 250mV
    //LDO_1_Set_Voltage(1000);


    while(1)
    {

        ////Fuel Gauge Parameter Reading

        printf("Cell Voltage: %f\n", Get_Cell_Voltage());

        printf("SOC: %f\n", Get_SOC());

        MXC_Delay(1000000);
    }

}





