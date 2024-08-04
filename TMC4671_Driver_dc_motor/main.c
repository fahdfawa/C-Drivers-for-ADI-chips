//////////////////////////MAX32660_TMC4671_TMC6100_CODE/////////////////////////
//////////////////////////Developed by Fahad Ahammad CAC APR/////////////////////

/***** Includes *****/
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
#include "led.h"
#include "pb.h"


#include "TMC4671_Registers.h"

/***** Definitions *****/

#define ENABLE_PORT MXC_GPIO0
#define ENABLE_PIN MXC_GPIO_PIN_7

#define CS2_PORT MXC_GPIO1
#define CS2_PIN MXC_GPIO_PIN_18

#define CS1_PORT MXC_GPIO2
#define CS1_PIN MXC_GPIO_PIN_26

/***** Globals *****/
uint8_t rx_data[DATA_LEN]={0};
uint8_t tx_data[DATA_LEN]={0};

mxc_spi_req_t req;

int32_t volatile i32SensorX;
int32_t volatile i32SensorY;
int32_t volatile i32SensorZ;
int32_t volatile i32SensorT;
uint32_t volatile ui32SensorX;
uint32_t volatile ui32SensorY;
uint32_t volatile ui32SensorZ;
uint32_t volatile ui32SensorT;

#define OPEN_LOOP
//#define CLOSED_LOOP
//#define VELOCITY_MODE
#define POSITION_MODE

/***** Functions *****/

// *****************************************************************************
int main(void)
{
	mxc_gpio_cfg_t  enable_out;
	mxc_gpio_cfg_t cs2_out;
	uint32_t Reg_Data = 0;
//	uint32_t Reg_Data2 = 0;

	////GPIO CONFIGURATION

	enable_out.port = ENABLE_PORT;
	enable_out.mask = ENABLE_PIN;
	enable_out.pad = MXC_GPIO_PAD_PULL_DOWN;
	enable_out.func = MXC_GPIO_FUNC_OUT;
	enable_out.vssel = MXC_GPIO_VSSEL_VDDIO;
    MXC_GPIO_Config(&enable_out);

	cs2_out.port = CS2_PORT;
	cs2_out.mask = CS2_PIN;
	cs2_out.pad = MXC_GPIO_PAD_PULL_UP;
	cs2_out.func = MXC_GPIO_FUNC_OUT;
	cs2_out.vssel = MXC_GPIO_VSSEL_VDDIO;
    MXC_GPIO_Config(&cs2_out);

    ///SPI CONFIGURATION
    mxc_spi_pins_t spi_pins;
    spi_pins.ss0 = FALSE;
	spi_pins.ss1 = FALSE;
	spi_pins.ss2 = FALSE;
    MXC_SPI_Init(SPI, 1, 0, 2, 0, SPI_SPEED,spi_pins);
	MXC_SPI_SetDataSize(SPI, 8);
	MXC_SPI_SetMode(SPI, SPI_MODE_3);   ///SPI_MODE3 for TMC4671
    memset(rx_data, 0x0, DATA_LEN * sizeof(uint8_t));

    //MXC_GPIO_OutSet(CS2_PORT, CS2_PIN);
    MXC_GPIO_OutClr(ENABLE_PORT, ENABLE_PIN);

   //GENERAL SETTINGS

    Write_Register_TMC4671(TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00010002);         //Motor type and No : of pole pairs
    Write_Register_TMC4671(TMC4671_PWM_MAXCNT, 0x00000F9F);                      //PWM switching frequency selection
    Write_Register_TMC4671(TMC4671_PWM_BBM_H_BBM_L, 0x00001919);                 //Break before make delay
    Write_Register_TMC4671(TMC4671_PWM_SV_CHOP, 0x00000007);                     //PWM type
    Write_Register_TMC4671(TMC4671_PWM_POLARITIES, 0x00000000);                  //PWM polarities

#ifdef OPEN_LOOP
    ///OPEN LOOP RUN SETTINGS

    Write_Register_TMC4671(TMC4671_MODE_RAMP_MODE_MOTION, 0x00000008);          //Mode Selection
    Write_Register_TMC4671(TMC4671_PHI_E_SELECTION, 0x00000002);                //Angle Selection for FOC Transformation
    Write_Register_TMC4671(TMC4671_OPENLOOP_MODE, 0x00000000);                  //PHI_E direction
    Write_Register_TMC4671(TMC4671_OPENLOOP_ACCELERATION, 0x0000010c);          //Open loop Acceleration Target
    Write_Register_TMC4671(TMC4671_OPENLOOP_VELOCITY_TARGET, 0x00000030);          //Velocity Target
    Write_Register_TMC4671(TMC4671_UQ_UD_EXT, 0x00001298);                      //Open loop voltage target

    //Enabling TMC4671 & TMC6100
    MXC_GPIO_OutSet(ENABLE_PORT, ENABLE_PIN);

#endif

#ifdef CLOSED_LOOP
//CLOSED LOOP SETTINGS

    // ADC configuration

	   Write_Register_TMC4671(TMC4671_ADC_I_SELECT, 0x09000100);                   //Assigning correct ADC channel to PWM channel
	   Write_Register_TMC4671(TMC4671_dsADC_MCFG_B_MCFG_A,0x00100010);            //Internal delta-sigma ADC configuration
	   Write_Register_TMC4671(TMC4671_dsADC_MCLK_A, 0x20000000);                   //Configuring delta sigma modulator clock
	   Write_Register_TMC4671(TMC4671_dsADC_MCLK_B, 0x20000000);                   //Configuring delta sigma modulator clock
	   Write_Register_TMC4671(TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);            // Decimation rates of SINC3 filter
	   Write_Register_TMC4671(TMC4671_ADC_I0_SCALE_OFFSET, 0x0100818B);            // Calibration of ADC channel 0
	   Write_Register_TMC4671(TMC4671_ADC_I1_SCALE_OFFSET, 0x01008211);            // Calibration of ADC channel 1

	   MXC_Delay(10000);

    // Feedback Selection and Configuration
//	   Write_Register_TMC4671(TMC4671_HALL_MODE,0x00000001);//0x33: polarity
//	   Write_Register_TMC4671(TMC4671_HALL_PHI_E_PHI_M_OFFSET,0x00000000);//0x37: Offset of mechanical angle hall_phi_m of hall decoder.

	   Write_Register_TMC4671(TMC4671_ABN_DECODER_MODE , 0x0001000);              //Configure Decoder input signals and count direction
	   Write_Register_TMC4671(TMC4671_ABN_DECODER_PPR, 0x000000C0);                //Decoder pulses per mechanical revolution
	   Write_Register_TMC4671(TMC4671_ABN_DECODER_COUNT, 0x00000096);              //Raw Decoder count
	   Write_Register_TMC4671(TMC4671_ABN_DECODER_COUNT_N, 0X00000096);            //Count value at N pulse event
	   Write_Register_TMC4671(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00008AD0); //Offset setting for angle from dec
//	   Write_Register_TMC4671(TMC4671_ABN_DECODER_PHI_E_PHI_M, 0xA59E52CF);        //Displays actual angle values for ABN encoder
//	   Write_Register_TMC4671(TMC4671_PID_POSITION_ACTUAL, 0x00000000);            //Actual position derived from chosen position signal.

	   MXC_Delay(10000);

	// Angle Selection
	   Write_Register_TMC4671(TMC4671_PHI_E_SELECTION, 0x00000003);                //Angle Selection for FOC Transformation
	   MXC_Delay(10000);

	//Flux & Torque PI parameters
//	   Write_Register_TMC4671(TMC4671_PIDOUT_UQ_UD_LIMITS, 0x00007FFF);
	   Write_Register_TMC4671(TMC4671_PID_FLUX_P_FLUX_I, 0x01000100);              //PI parameters for Flux controller
	   Write_Register_TMC4671(TMC4671_PID_TORQUE_P_TORQUE_I, 0x03E82710);          //PI parameters for Torque controller
	   Write_Register_TMC4671(TMC4671_PID_TORQUE_FLUX_LIMITS, 0x000003E8);           //Target current limit for Flux & Torque controller
	   Write_Register_TMC4671(TMC4671_PID_VELOCITY_LIMIT, 0x000003E8);
//	   Write_Register_TMC4671(TMC4671_PID_ACCELERATION_LIMIT, 0x000000C8);

	// Velocity & Position PI parameters
	   Write_Register_TMC4671(TMC4671_PID_VELOCITY_P_VELOCITY_I, 0x03200064);      //PI parameters for Velocity loop
	   Write_Register_TMC4671(TMC4671_PID_POSITION_P_POSITION_I, 0x000A0000);      //PI parameters for Position loop


    #ifdef VELOCITY_MODE
	  int32_t actual_velocity;
	  int32_t target_velocity;

	  do{
		Write_Register_TMC4671(TMC4671_MODE_RAMP_MODE_MOTION, 2);
		actual_velocity = Read_Register_TMC4671(TMC4671_PID_VELOCITY_ACTUAL);
		Write_Register_TMC4671(TMC4671_PID_VELOCITY_TARGET, 0x00000032);
		target_velocity = Read_Register_TMC4671(TMC4671_PID_VELOCITY_TARGET);
	    }while(actual_velocity != target_velocity);

	    MXC_GPIO_OutSet(ENABLE_PORT, ENABLE_PIN);
	  #endif

    #ifdef POSITION_MODE

        uint32_t actualModeRegister = Read_Register_TMC4671(TMC4671_MODE_RAMP_MODE_MOTION);
    	actualModeRegister &= 0xFFFFFF00;
    	actualModeRegister |= 3;

    	Write_Register_TMC4671(TMC4671_MODE_RAMP_MODE_MOTION, actualModeRegister);        //Mode Selection

    	Write_Register_TMC4671(TMC4671_PID_POSITION_TARGET, 0x00A0000);                                  //Position Target

        MXC_GPIO_OutSet(ENABLE_PORT, ENABLE_PIN);                                                               //Enabling the driver

    #endif

#endif

    Read_Register_TMC4671(TMC4671_MOTOR_TYPE_N_POLE_PAIRS);
    Reg_Data = Read_Register_TMC4671(TMC4671_MOTOR_TYPE_N_POLE_PAIRS);
    printf("Confirmation TMC4671: %x\n", Reg_Data);

    //CLOSED LOOP SETTINGS


//    Read_Register_TMC6100(0x00);
//    Reg_Data2 = Read_Register_TMC6100(0x00);
//        printf("Confirmation TMC6100: %x\n", Reg_Data2);



}

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
	req.ssDeassert = 0; //Chip select to high after transaction
	MXC_GPIO_OutClr(CS1_PORT, CS1_PIN);
	MXC_SPI_MasterTransaction(&req); //Maxim inbuilt function for SPI Transfer
	MXC_GPIO_OutSet(CS1_PORT, CS1_PIN);


}

void SPI_ReadWrite_TMC6100(uint8_t *tx_data, uint8_t *rx_data, uint8_t TotalNoOfBytes)
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
	req.ssDeassert = 0; //Chip select to low after transaction
	MXC_GPIO_OutClr(CS2_PORT, CS2_PIN);
	MXC_SPI_MasterTransaction(&req); //Maxim inbuilt function for SPI Transfer
	MXC_GPIO_OutSet(CS2_PORT, CS2_PIN);
}

void Write_Register_TMC4671(uint8_t Addr, uint32_t Data)
{
  uint8_t Byte_Count = 5;
  tx_data[0] = 0xFF & (0x80 | Addr);
  tx_data[1] = 0xFF & Data >> 24;
  tx_data[2] = 0xFF & Data >> 16;
  tx_data[3] = 0xFF & Data >> 8;
  tx_data[4] = 0xFF & Data;


  SPI_ReadWrite(tx_data, rx_data, Byte_Count);
}

uint32_t Read_Register_TMC4671(uint8_t Addr)
{
  uint32_t Return_value = 0;
  tx_data[0] = Addr;
  tx_data[1] = 0x00;
  tx_data[2] = 0x00;
  tx_data[3] = 0x00;
  tx_data[4] = 0x00;
  SPI_ReadWrite(tx_data, rx_data, 5);
  return Return_value =  0xFFFFFFFF & ((rx_data[4]) | (rx_data[3] << 8) | (rx_data[2] << 16) | (rx_data[1] << 24));
}


void Write_Register_TMC6100(uint8_t Addr, uint32_t Data)
{
  uint8_t Byte_Count = 5;
  tx_data[0] = 0xFF & (0x80 | Addr);
  tx_data[1] = 0xFF & Data >> 24;
  tx_data[2] = 0xFF & Data >> 16;
  tx_data[3] = 0xFF & Data >> 8;
  tx_data[4] = 0xFF & Data;

  printf("tx_data: %x\n",tx_data);

  SPI_ReadWrite_TMC6100(tx_data, rx_data, Byte_Count);
}

uint32_t Read_Register_TMC6100(uint8_t Addr)
{
  tx_data[0] = Addr;
  tx_data[1] = 0x00;
  tx_data[2] = 0x00;
  tx_data[3] = 0x00;
  tx_data[4] = 0x00;
  SPI_ReadWrite_TMC6100(tx_data, rx_data, 5);
  uint32_t Return =  0xFFFFFFFF & (rx_data[4] | rx_data[3] << 8 | rx_data[2] << 16 | rx_data[1] << 24);
  return Return;
}

