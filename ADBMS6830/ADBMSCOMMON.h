

#ifndef ADBMSCOMMON_H_
#define ADBMSCOMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include "spi_reva.h"

/*=============D E F I N E S =============*/

/***** Initialization of BMS IC *****/
#define 	TOTAL_IC		1

/***** Initialization of MCU *****/
#define 	SPI_SPEED       500000		//Clock speed for ADBMS6830 //LTC6820 - Mx CLK Speed 1MHz // ADBMS6821/22 - Mx CLK Speed 2MHz
#define		Master_Mode		1			//MCU in Master Mode
#define		Slave_Mode		0			//MCU in Slave Mode (Target in Master Mode)
#define		Quad_SPI		1			//4-bits per SCK cycle (Quad mode SPI).
#define		Single_SPI		0			//1-bit per SCK cycle (Three-wire half-duplex SPI and Four-wire full-duplex SPI)
#define 	Slave_1			1			//1 Slave  connected to the MCU
#define 	Slave_2			2			//2 Slaves connected to the MCU
#define 	Slave_3			3			//3 Slaves connected to the MCU
#define 	Slave_4			4			//4 Slaves connected to the MCU
#define		SS_Polarity		0         	//Slave Select at Active low polarity

///**** Code Test *****///
#define Test_Print      	0

/***** BMS IC *****/
/*Number of cells*/
#define N_CELLS 16U
/*Number of cells*/
#define N_CELLS_PER_REGISTERS 3U
/*Number of GPIOs*/
#define N_GPIOS 10U
/*Number of bytes for a cell*/
#define BYTES_IN_CELL 2U
/*Number of bytes in data PEC*/
#define PEC_SIZE 2U
/*Number of registers in a group*/
#define REGISTER_CNT_IN_GRP 6U
/*Number of bytes receive for Cell, S, Aux, Status Read Commands*/
#define RECEIVE_RD_PACKET_SIZE ((N_CELLS_PER_REGISTERS * BYTES_IN_CELL) + PEC_SIZE)*TOTAL_IC
/*Number of bytes receive for All Read Commands*/
#define RECEIVE_RD_ALL_PACKET_SIZE ((N_CELLS * BYTES_IN_CELL) + PEC_SIZE)   //  32 + 2
/*Number of bytes on the command packet 2byte Cmd and 2 byte PEC*/
#define COMMAND_PACKET_SIZE 4U
/*Number of byte receive for RDCVALL command*/
#define RDCVALL_SIZE  RECEIVE_RD_ALL_PACKET_SIZE                //  32 + 2
/*Number of byte receive for RDACVALL command*/
#define RDACVALL_SIZE  RECEIVE_RD_ALL_PACKET_SIZE               //  32 + 2
/*Number of byte receive for RDSVALL command*/
#define RDSVALL_SIZE  RECEIVE_RD_ALL_PACKET_SIZE                //  32 + 2
/*Number of byte receive for RDFCVALL command*/
#define RDFCVALL_SIZE  RECEIVE_RD_ALL_PACKET_SIZE               //  32 + 2
/*Number of Bytes for Read All GPIOs*/
#define RDGPALL_SIZE ((N_GPIOS * BYTES_IN_CELL) + PEC_SIZE)       //  22 + 2                          
#define RDAUXALL_SIZE ((N_GPIOS * BYTES_IN_CELL) + PEC_SIZE)      //  22 + 2
/*Number of Bytes for Read All Redundant GPIOs*/
#define RDRGPALL_SIZE ((N_GPIOS * BYTES_IN_CELL) + PEC_SIZE)      //  22 + 2
/*Number of Bytes for RDCSALL command*/
#define RDCSALL_SIZE (((N_CELLS * BYTES_IN_CELL) * 2) + PEC_SIZE)     //  32*2 + 2
/*Number of Bytes for RDACSALL command*/
#define RDACSALL_SIZE (((N_CELLS * BYTES_IN_CELL) * 2) + PEC_SIZE)     //  32*2 + 2
/*Number of Bytes for RDACSALL command*/
#define RDSTAALL_SIZE 0
/*Number of Bytes for RDACSALL command*/
#define RDCCFGALL_SIZE 0

///**** Definations *****///
#define ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x)    ((double)(x) * (double)(0.000150) + (double)(1.5))  //Cell Voltage
#define ADBMS_CONVERT_GPIO_HEX_TO_VOLT(x)           ((double)(x) * (double)(0.000150) + (double)(1.5))  //GPIO(AUX) Voltage
//STATA
#define ADBMS_CONVERT_VREF2_HEX_TO_VOLT(x)          ((double)(x) * (double)(0.000150) + (double)(1.5))  //VREF2 (2.988V to 3.012V)
#define ADBMS_CONVERT_ITEMP_HEX_TO_VOLT(x)          ((double)(x) * (double)(0.000150) + (double)(1.5))/(double)(0.0075) - (double)(273)  //Dia Temp of IC
//STATB
#define ADBMS_CONVERT_VD_HEX_TO_VOLT(x)             ((double)(x) * (double)(0.000150) + (double)(1.5))  //Digital power supply (2.7 to 3.6V)
#define ADBMS_CONVERT_VA_HEX_TO_VOLT(x)             ((double)(x) * (double)(0.000150) + (double)(1.5))  //Analog Power Supply (4.5V to 5.5V)
#define ADBMS_CONVERT_VRES_HEX_TO_VOLT(x)           ((double)(x) * (double)(0.000150) + (double)(1.5))  //Voltage Accross 4K Resistor (2.9V to 3.1V)
//AUX4//GPIO4
#define ADBMS_CONVERT_VM_HEX_TO_VOLT(x)             ((double)(x) * (double)(0.000150) + (double)(1.5))  //Voltage Accross -V and Exposed PAD (~0V)
#define ADBMS_CONVERT_VP_HEX_TO_VOLT(x)             ((double)(x) * (double)(0.000150) + (double)(1.5))*(double)(25)  //Total Cell Voltage(12V to 72V) (+v to -V)

///***** Set ADCV CMD *****/
//ADBMS_ADCV(CONT_ADCV, RD_OFF_ADCV, NO_RESET_ADCV, NODISCHARGE_ADCV, ALL_CH_OW_OFF_ADCV, &cmd[0]);
#define CONT_ADCV					    0x1
#define SINGLESHOT_ADCV					0x0
#define RD_OFF_ADCV					  	0x0
#define RD_ON_ADCV					  	0x1
#define NO_RESET_ADCV				  	0x0
#define RESET_ADCV					  	0x0
#define DISCHARGE_ADCV					0x1
#define NODISCHARGE_ADCV				0x0
#define DISCHARGE_ADCV					0x1
#define ALL_CH_OW_OFF_ADCV				0x0
#define EVEN_CH_OW_ADCV					0x1
#define ODD_CH_OW_ADCV					0x2
#define ALL_CH_OW_OFF2_ADCV				0x3

///***** Set ADSV CMD *****/
//ADBMS_ADSV(CONT_ADSV, NODISCHARGE_ADSV, ALL_CH_OW_OFF_ADSV, &cmd[0]);
#define CONT_ADSV					    0x1
#define Singleshot_ADSV					0x0
#define DISCHARGE_ADSV					0x1
#define NODISCHARGE_ADSV				0x0
#define ALL_CH_OW_OFF_ADSV				0x0
#define EVEN_CH_OW_ADSV					0x1
#define ODD_CH_OW_ADSV					0x2
#define ALL_CH_OW_OFF2_ADSV				0x3

///***** Set ADAX amd ADAX2 CMD *****/
// void ADBMS_ADAX(uint8_t AUX_OW, uint8_t CH_ADAX, uint8_t PUP, uint8_t *cmd);
// void ADBMS_ADAX2(uint8_t CH_ADAX2, uint8_t *cmd);
#define OW_AUX_OFF_ADAX       0x0
#define OW_AUX_ON_ADAX        0x1
#define PULLDOWN_ADAX         0x0
#define PULLUP_ADAX           0x1
/*! \enum   CH_ADAX_t
    \brief  Channel Select
*/
typedef enum
{
  ALL_ADAX = 0x0,	              /*!<All Channel Select*/
  GPIO1_ADAX = 0x1,	            /*!<Select GPIO 1*/
  GPIO2_ADAX = 0x2,	            /*!<Select GPIO2*/
  GPIO3_ADAX = 0x3,	            /*!<Select GPIO3*/
  GPIO4_ADAX = 0x4,	            /*!<Select GPIO4*/
  GPIO5_ADAX = 0x5,	            /*!<Select GPIO5*/
  GPIO6_ADAX = 0x6,	            /*!<Select GPIO6*/
  GPIO7_ADAX = 0x7,	            /*!<Selet GPIO7*/
  GPIO8_ADAX = 0x8,	            /*!<Select GPIO8*/
  GPIO9_ADAX = 0x9,	            /*!<Select GPIO9*/
  GPIO10_ADAX = 0xA,	          /*!<Select GPIO10*/
  VREF2_ADAX = 0x10,	          /*!<Select Vref2*/
  VD_ADAX = 0x11,	          /*!<Select VD*/
  VA_ADAX = 0x12,	          /*!<Select VA*/
  TEMP_ADAX = 0x13,	            /*!<Select TEMP*/
  VPV_ADAX = 0x14,	            /*!<Select VPV*/
  VMV_ADAX = 0x15,	              /*!<Select VMV*/
  VRES_ADAX = 0x16,	            /*!<Select VRES*/
}CH_ADAX_t;

/*! \enum   CH_ADAX2_t
    \brief  Channel Select
*/
typedef enum
{
  ALL_ADAX2 = 0x0,	        /*!<All Channel Select*/
  GPIO8TO10_ADAX2 = 0xF,	  /*!<GPIO 8-10*/
  GPIO1TO7_ADAX2 = 0xE,	    /*!<GPIO 1-7*/
  GPIO1_ADAX2 = 0x1,	      /*!<Select GPIO 1*/
  GPIO2_ADAX2 = 0x2,	      /*!<Select GPIO2*/
  GPIO3_ADAX2 = 0x3,	      /*!<Select GPIO3*/
  GPIO4_ADAX2 = 0x4,	      /*!<Select GPIO4*/
  GPIO5_ADAX2 = 0x5,	      /*!<Select GPIO5*/
  GPIO6_ADAX2 = 0x6,	      /*!<Select GPIO6*/
  GPIO7_ADAX2 = 0x7,	      /*!<Selet GPIO7*/
  GPIO8_ADAX2 = 0x8,	      /*!<Select GPIO8*/
  GPIO9_ADAX2 = 0x9,	      /*!<Select GPIO9*/
  GPIO10_ADAX2 = 0xA	      /*!<Select GPIO10*/
}CH_ADAX2_t;

typedef enum
{
RD_CV_ALL,        /*!< Read All Cell Voltage Result Registers*/
RD_AC_ALL,        /*!< Read All Averaged Cell Voltage Result Registers*/
RD_S_ALL,         /*!< Read All S-Voltage Result Registers*/
RD_FC_ALL,        /*!< Read All Filtered Cell Voltage Result Registers*/
RD_AUX_ALL,
RD_C_S_ALL,       /*!< Read all C & S Results*/
RD_AC_S_ALL,      /*!< Read all Avg (C & S) Results*/
RD_STA_ALL,		    /*!< Read All Status Registers Group*/
RD_C_CFG_ALL,		  /*!< Read All Configuration Registers Group*/
NOT_ALL,
GRP_NONE,
GRPA,
GRPB,
GRPC,
GRPD,
GRPE,
GRPF
}RD_DATA_SIZE_ALL_TYPE;

/*=============P U B L I C P R O T O T Y P E S =============*/

/*============= Function Declaration  =============*/
//main.c
void SPI_Transaction(uint8_t *tx_data, uint8_t *rx_data, uint8_t data_size);
int WakeupIC(mxc_spi_regs_t *spi);
void WakeupBMSIC(void);

//ADBMSMAIN.C
void adbmsmain(void);
void printMenu(void);
void run_command(int command);
void ADBMS_Init(uint8_t tIC);
void ADBMS_Write_Read_Config(uint8_t tIC);
void ADC_Cell_Voltage_Measurement(uint8_t tIC);
void ADC_S_Voltage_Measurement(uint8_t tIC);
void ADC_AVG_Cell_Voltage_Measurement(uint8_t tIC);
void ADC_GPIO_Voltage_Measurement(uint8_t tIC);
void ADBMS_Status_Reg_voltage_measurment(uint8_t tIC);
void ADSV_PWM_Discharge_Measurement(uint8_t tIC);
void ADBMS_Print_WRCFG_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type);
void ADBMS_Print_RDCFG_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type);
void ADBMS_Print_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type);
void ADBMS_Print_AUX_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type);
void ADBMS_Print_Status_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type);
void ADBMSPollADC(uint8_t tIC, uint8_t cmd_arg[2]);
void pollVoltCTSconversion(void);
void poll8msCtsVconversion(void);

//ADBMSCOMMON.c
void SPI_Write(uint8_t tIC, uint8_t cmd_arg[0]);
void ADBMS_Write_Cmd(uint8_t tIC, uint8_t cmd_arg[2]);
void ADBMS_Write_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *data_Write);
void ADBMS_Read_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *data_Read, RD_DATA_SIZE_ALL_TYPE type);
uint16_t Pec15_Calc( uint8_t len, uint8_t *data);
uint16_t pec10_calc(bool rx_cmd, int len, uint8_t *data);
int16_t twos_complement_to_int(uint16_t value, uint8_t num_bits);
uint16_t SetUnderVoltageThreshold(double voltage);
uint16_t SetOverVoltageThreshold(double voltage);
void WRCFGB_data_Set(uint8_t tIC, uint8_t cmd_arg[], float UV_THSD, float OV_THSD, uint8_t *data_Write);

//ADBMSCMDLIST.h
void ADBMS_ADCV(uint8_t CV_CONT, uint8_t CV_RD, uint8_t CV_RSTF, uint8_t CV_SSDP, uint8_t C_OW, uint8_t *cmd);
void ADBMS_ADSV(uint8_t SV_CONT, uint8_t SV_SSDP, uint8_t S_OW, uint8_t *cmd);
void ADBMS_ADAX(uint8_t AUX_OW, CH_ADAX_t CH_ADAX, uint8_t PUP, uint8_t *cmd);
void ADBMS_ADAX2(CH_ADAX2_t CH_ADAX2, uint8_t *cmd);
void ADBMS_RDSTATC(uint8_t ERR, uint8_t *cmd);

#endif /* ADBMSCOMMON_H_ */
