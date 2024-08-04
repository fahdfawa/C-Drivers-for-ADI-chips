////////////MAX32660 code for MAX77958 PD Negotiator////////////////////
//////////////Developed by Fahad Ahammad CAC APR////////////////////////

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "gpio.h"
#include "i2c.h"
#include "dma.h"
#include "led.h"
#include "FUNCTIONs.h"


/***** Definitions *****/
//void Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value);
//void Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB);
////void Register_Multi_Read(mxc_i2c_regs_t* i2c_master, uint8_t Read_Addr_strt, uint8_t Read_Addr_end, uint8_t* Read_value, uint8_t* rx_buffer);
//void Register_Multi_Read(mxc_i2c_regs_t* i2c_master, uint8_t start_reg, uint8_t num_regs, uint8_t* Read_values);
// #define MASTERDMA


//#define I2C_SLAVE MXC_I2C1

#define I2C_FREQ 100000
#define I2C_BYTES 100



/***** Globals *****/

static uint8_t txdata[I2C_BYTES];
static uint8_t rxdata[I2C_BYTES];
uint8_t tx_buffer[50];
volatile uint8_t DMA_FLAG = 0;
volatile int I2C_FLAG;
volatile int txnum = 0;
volatile int txcnt = 0;
volatile int rxnum = 0;
volatile int num;





//Prints out human-friendly format to read txdata and rxdata
void printData(void)
{
    int i;
    printf("\n-->TxData: ");

    for (i = 0; i < sizeof(txdata); ++i) {
        printf("%02x ", txdata[i]);
    }

    printf("\n\n-->RxData: ");

    for (i = 0; i < sizeof(rxdata); ++i) {
        printf("%02x ", rxdata[i]);
    }

    printf("\n");

    return;
}


// *****************************************************************************
int main()
{

    int error = 0;

    //Setup the I2CM
    error = MXC_I2C_Init(I2C_MASTER, 1, 0);

    if (error != E_NO_ERROR) {
        printf("-->Failed master\n");
        return error;
    } else {
        printf("\n-->I2C Master Initialization Complete\n");
    }

    NVIC_EnableIRQ(I2C0_IRQn);

    MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ);

   //GPIO Interrupt Settings

    GPIO_Interrupt_Enable();


    Set_Interrupt_Mask();


    ///Union instances

    Current_Src_Cap_union curr_src_cap_combined;
    Src_Cap_Req_union src_cap_req_combined;
    SNK_PDO_union snk_PDO_req_combined;
    uint32_t src_PDO1, src_PDO2, src_PDO3, src_PDO4, src_PDO5, src_PDO6, src_PDO7, src_PDO8;
	uint8_t Number_of_PDOs;
	uint8_t Selected_PDO;

	//Calling twice is required due to interrupt clearing
	/**
	 *
	 * @brief   Reading the available source PDOs of the charger
	 *
	 * @return  Number of PDOs, Sink PDOs in hex value
	 */
	Current_Source_Cap(&curr_src_cap_combined, &Number_of_PDOs, &Selected_PDO, &src_PDO1, &src_PDO2, &src_PDO3, &src_PDO4, &src_PDO5, &src_PDO6, &src_PDO7, &src_PDO8);


	printf("Selected Source PDOs: %X\n", Selected_PDO);
    printf("Source_PDO1: %X\n", src_PDO1);
	printf("Source_PDO2:%X\n", src_PDO2);
	printf("Source_PDO3:%X\n", src_PDO3);
	printf("Source_PDO4:%X\n", src_PDO4);
	printf("Source_PDO5:%X\n", src_PDO5);
	printf("Source_PDO6:%X\n", src_PDO6);
	printf("Source_PDO7:%X\n", src_PDO7);
	printf("Source_PDO8:%X\n", src_PDO8);

	uint8_t Num_PDOs;

	uint32_t snk_PDO1, snk_PDO2, snk_PDO3, snk_PDO4, snk_PDO5, snk_PDO6;

	/**
	 * @brief   Reading the sink PDOs available
	 *
	 * @return  Number of PDOs, Sink PDOs in hex value
	 */
	Sink_PDO_Req(&snk_PDO_req_combined, &Num_PDOs, &snk_PDO1, &snk_PDO2, &snk_PDO3, &snk_PDO4, &snk_PDO5, &snk_PDO6);


	/**
	 * @brief   Setting Sink PDO for initial voltage configuration during charger plugin
	 *
	 * @param   memory write, No of PDOs, Sink PDO inputs in hex values (refer user guide for more details)
	 */
    Set_Sink_PDOs(MTP_Write, Position1, 0x1401912c, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);

	printf("No: of Sink PDOs: %X\n", Num_PDOs);
	printf("Sink_PDO1: %X\n", snk_PDO1);
	printf("Sink_PDO2: %X\n", snk_PDO2);
	printf("Sink_PDO3: %X\n", snk_PDO3);
	printf("Sink_PDO4: %X\n", snk_PDO4);
	printf("Sink_PDO5: %X\n", snk_PDO5);
	printf("Sink_PDO6: %X\n", snk_PDO6);

	uint8_t Voltage_value, Current_value, PD_Status_value;
	/**
	 * @brief   Read the charger details
	 *
	 * @return  VBUS ADC Voltage read, Current limit, Status of connection & Charger
	 */
	Port_Detection_Status_Voltage_current_Read(&Voltage_value, &Current_value, &PD_Status_value);

    uint16_t Terminal_Voltage = 8400; //in mV
    uint16_t Input_curr_lim  = 2000; //in mA
    uint16_t Fast_chg_curr = 2500;  //in mA



    /**
     * @brief   Configuring MAX77962 Charger terminal voltage, Input USB Current limit, Fast charging current
     *
     * @param   Terminal voltage, Input current limit, Fast charging current
     *
     * @return  The I2C bus frequency in Hertz
     */
    MAX77962_Charger_initial_settings(Terminal_Voltage, Input_curr_lim, Fast_chg_curr);



    /**
     * @brief  Change the PDO Voltage
     *
     * @param   Position: Each position corresponds to each PDO voltage
     *
     * @return  No return
     */
    //Source_Cap_Req(Position4, &src_cap_req_combined);



//   MXC_Delay(2000000);

while(1)
  {
    {
    }

  }


}









