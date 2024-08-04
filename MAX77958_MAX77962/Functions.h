

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "Registers.h"
#include "Fields.h"
#include <stdint.h>
#include <stdio.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "gpio.h"
#include "led.h"

//Creating Instance
mxc_i2c_req_t reqMaster;
uint8_t USB_STATUS_1_Read[1];
uint8_t CC_Istat[1];
uint8_t Voltage_Read;
uint8_t Current_Read;

//Function Declaration



typedef struct{

	unsigned int Req_PDO_Pos : 3;
	unsigned int Reserved    : 5;

}Src_Cap_Req;

typedef union{

	Src_Cap_Req src_cap_req;
	uint8_t Src_Cap_Req_combined;

}Src_Cap_Req_union;

typedef struct{

  uint8_t Command;
  uint8_t Conf;
  uint8_t PDO1_1;
  uint8_t PDO1_2;
  uint8_t PDO1_3;
  uint8_t PDO1_4;
  uint8_t PDO2_1;
  uint8_t PDO2_2;
  uint8_t PDO2_3;
  uint8_t PDO2_4;
  uint8_t PDO3_1;
  uint8_t PDO3_2;
  uint8_t PDO3_3;
  uint8_t PDO3_4;
  uint8_t PDO4_1;
  uint8_t PDO4_2;
  uint8_t PDO4_3;
  uint8_t PDO4_4;
  uint8_t PDO5_1;
  uint8_t PDO5_2;
  uint8_t PDO5_3;
  uint8_t PDO5_4;
  uint8_t PDO6_1;
  uint8_t PDO6_2;
  uint8_t PDO6_3;
  uint8_t PDO6_4;
  uint8_t PDO7_1;
  uint8_t PDO7_2;
  uint8_t PDO7_3;
  uint8_t PDO7_4;
  uint8_t PDO8_1;
  uint8_t PDO8_2;
  uint8_t PDO8_3;
  uint8_t PDO8_4;


}Current_Src_Cap;


typedef union{

	Current_Src_Cap cur_src_cap;
	uint8_t Current_Src_Cap_combined[40];

}Current_Src_Cap_union;

typedef struct{

  uint8_t Command;
  uint8_t No_of_PDOs;
  uint8_t PDO1_1;
  uint8_t PDO1_2;
  uint8_t PDO1_3;
  uint8_t PDO1_4;
  uint8_t PDO2_1;
  uint8_t PDO2_2;
  uint8_t PDO2_3;
  uint8_t PDO2_4;
  uint8_t PDO3_1;
  uint8_t PDO3_2;
  uint8_t PDO3_3;
  uint8_t PDO3_4;
  uint8_t PDO4_1;
  uint8_t PDO4_2;
  uint8_t PDO4_3;
  uint8_t PDO4_4;
  uint8_t PDO5_1;
  uint8_t PDO5_2;
  uint8_t PDO5_3;
  uint8_t PDO5_4;
  uint8_t PDO6_1;
  uint8_t PDO6_2;
  uint8_t PDO6_3;
  uint8_t PDO6_4;

}SNK_PDO;


typedef union{

	SNK_PDO snk_pdo;
	uint8_t SNK_PDO_Combined[40];

}SNK_PDO_union;


typedef struct
{

	unsigned int CHGDetEn        : 1;
	unsigned int CHGDetMan       : 1;
	unsigned int                 : 1;
	unsigned int Nikon_Detection : 1;
	unsigned int                 : 4;
	unsigned int DCDCpl          : 1;

}BC_CTRL1;

typedef union{

	BC_CTRL1 bc_ctrl1;
	uint8_t BC_CTRL1_combined;

}BC_CTRL_union;


typedef struct{

	unsigned int AttachedHoldM : 1;
	unsigned int ChgTypM       : 1;
	unsigned int StopModeM     : 1;
	unsigned int DCDTmoM       : 1;
	unsigned int VbADCM        : 1;
	unsigned int VBUSDetM      : 1;
	unsigned int SYSMsgM       : 1;
	unsigned int APCmdResM     : 1;

}UIC_Int_Msk;

typedef union{

	UIC_Int_Msk uic_int_mask;
	uint8_t UIC_Int_Msk_combined;

}UIC_Int_Msk_union;

typedef struct{

	unsigned int AttachedHoldI : 1;
	unsigned int ChgTypI       : 1;
	unsigned int StopModeI     : 1;
	unsigned int DCDTmoI       : 1;
	unsigned int VbADCI        : 1;
	unsigned int VBUSDetI      : 1;
	unsigned int SYSMsgI       : 1;
	unsigned int APCmdResI     : 1;

}UIC_Int;

typedef union{

	UIC_Int uic_int;
	uint8_t UIC_Int_combined;

}UIC_Int_union;







////Instance declaration

Current_Src_Cap_union curr_src_cap_combined;
Src_Cap_Req_union src_cap_req_combined;
SNK_PDO_union snk_PDO_req_combined;







void Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value)
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x25;
	reqMaster.tx_buf = &reg;                                     //tx_buf is a pointer
	reqMaster.tx_len = sizeof(reg);
	reqMaster.rx_buf = Read_value;                               //
	reqMaster.rx_len = 1;                                        //rx_len !=0 fir read operation
	MXC_I2C_MasterTransaction(&reqMaster);

}

void Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB) //
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x25;
	uint8_t buf[2] = {reg, Write_value0_LSB}; // is write data is more than 16bit, add Write_value1_LSB, Write_value1_MSB and so on...
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_len = 0;                                        //rx_len = 0 for write operation
	MXC_I2C_MasterTransaction(&reqMaster);
}


void Register_Multi_Read(mxc_i2c_regs_t* i2c_master, uint8_t start_reg, uint8_t num_regs, uint8_t* Read_values)
{

    for (uint8_t i = 0; i < num_regs; i++) {
        Register_Read(i2c_master, start_reg + i, &Read_values[i]);
    }
}

void Multi_Register_4byte_Write(mxc_i2c_regs_t* i2c_master, uint8_t start_reg, uint32_t Write_Value_32_bit){
	    uint8_t write_buff[4];

	    write_buff[0] = (uint8_t)(0xFF & Write_Value_32_bit);        // Least significant byte (LSB)
	    write_buff[1] = (uint8_t)(0xFF & Write_Value_32_bit >> 8);
	    write_buff[2] = (uint8_t)(0xFF & Write_Value_32_bit >> 16);
	    write_buff[3] = (uint8_t)(0xFF & Write_Value_32_bit >> 24);

	for (uint8_t i = 0; i < 4; i++) {
	     Register_Write(i2c_master, start_reg + i, write_buff[i]);
	}
}





void BC_CTRL1_Write(uint8_t DCDCpt, State Nikon_det, State CHGDetMan, State CHGDetEn, BC_CTRL_union *comb_bc_ctrl1 ){

	comb_bc_ctrl1->bc_ctrl1.CHGDetEn  = CHGDetEn;
	comb_bc_ctrl1->bc_ctrl1.CHGDetMan = CHGDetMan;
	comb_bc_ctrl1->bc_ctrl1.Nikon_Detection = Nikon_det;
	comb_bc_ctrl1->bc_ctrl1.DCDCpl   = DCDCpt;

	Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, BC_CTRL1_CONFIG_WRITE);
	Register_Write(I2C_MASTER, COMMAND_WRITE_DATA_0, comb_bc_ctrl1->BC_CTRL1_combined);
	Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);
}


void BC_CTRL1_Read(BC_CTRL_union *comb_bc_ctrl1, uint8_t* BC_CTRL1_value){

	Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, BC_CTRL1_CONFIG_WRITE);
	Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);

	Register_Read(I2C_MASTER, COMMAND_READ_ADDRESS ,comb_bc_ctrl1->BC_CTRL1_combined);
	*BC_CTRL1_value =  comb_bc_ctrl1->BC_CTRL1_combined;

}

void Source_Cap_Req(PDO_position Position,  Src_Cap_Req_union *comb_src_cap_req){

	 comb_src_cap_req->src_cap_req.Req_PDO_Pos = Position;
	 Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, SRC_CAP_REQ);
	 Register_Write(I2C_MASTER, COMMAND_WRITE_DATA_0, comb_src_cap_req->Src_Cap_Req_combined);
	 Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);
}


void Current_Source_Cap(Current_Src_Cap_union *comb_current_src_cap, uint8_t *No_of_PDOs, uint8_t *Selected_Pos, uint32_t* SRC_PDO1, uint32_t* SRC_PDO2, uint32_t* SRC_PDO3, uint32_t* SRC_PDO4, uint32_t* SRC_PDO5, uint32_t* SRC_PDO6, uint32_t* SRC_PDO7, uint32_t* SRC_PDO8){

	uint8_t NO_OF_READ_ADDRESS = 36;
	Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, CUR_SEL_SRC_CAP);
	Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);
	Register_Multi_Read(I2C_MASTER, COMMAND_READ_ADDRESS, NO_OF_READ_ADDRESS, comb_current_src_cap->Current_Src_Cap_combined);
	*No_of_PDOs = comb_current_src_cap->cur_src_cap.Conf & 0b00000111;
	*Selected_Pos = (comb_current_src_cap->cur_src_cap.Conf >> 3) & 0b00000111;
    *SRC_PDO1 = comb_current_src_cap->cur_src_cap.PDO1_1 | comb_current_src_cap->cur_src_cap.PDO1_2 << 8 | comb_current_src_cap->cur_src_cap.PDO1_3 << 16 | comb_current_src_cap->cur_src_cap.PDO1_4 << 24 ;
    *SRC_PDO2 = comb_current_src_cap->cur_src_cap.PDO2_1 | comb_current_src_cap->cur_src_cap.PDO2_2 << 8 | comb_current_src_cap->cur_src_cap.PDO2_3 << 16 | comb_current_src_cap->cur_src_cap.PDO2_4 << 24 ;
    *SRC_PDO3 = comb_current_src_cap->cur_src_cap.PDO3_1 | comb_current_src_cap->cur_src_cap.PDO3_2 << 8 | comb_current_src_cap->cur_src_cap.PDO3_3 << 16 | comb_current_src_cap->cur_src_cap.PDO3_4 << 24 ;
    *SRC_PDO4 = comb_current_src_cap->cur_src_cap.PDO4_1 | comb_current_src_cap->cur_src_cap.PDO4_2 << 8 | comb_current_src_cap->cur_src_cap.PDO4_3 << 16 | comb_current_src_cap->cur_src_cap.PDO4_4 << 24 ;
    *SRC_PDO5 = comb_current_src_cap->cur_src_cap.PDO5_1 | comb_current_src_cap->cur_src_cap.PDO5_2 << 8 | comb_current_src_cap->cur_src_cap.PDO5_3 << 16 | comb_current_src_cap->cur_src_cap.PDO5_4 << 24 ;
	*SRC_PDO6 = comb_current_src_cap->cur_src_cap.PDO6_1 | comb_current_src_cap->cur_src_cap.PDO6_2 << 8 | comb_current_src_cap->cur_src_cap.PDO6_3 << 16 | comb_current_src_cap->cur_src_cap.PDO6_4 << 24 ;
	*SRC_PDO7 = comb_current_src_cap->cur_src_cap.PDO7_1 | comb_current_src_cap->cur_src_cap.PDO7_2 << 8 | comb_current_src_cap->cur_src_cap.PDO7_3 << 16 | comb_current_src_cap->cur_src_cap.PDO7_4 << 24 ;
	*SRC_PDO8 = comb_current_src_cap->cur_src_cap.PDO8_1 | comb_current_src_cap->cur_src_cap.PDO8_2 << 8 | comb_current_src_cap->cur_src_cap.PDO8_3 << 16 | comb_current_src_cap->cur_src_cap.PDO8_4 << 24 ;

}

void Set_Sink_PDOs(Memory_Write mry_write, PDO_position No_of_PDOs, uint32_t Snk_PDO1, uint32_t Snk_PDO2, uint32_t Snk_PDO3, uint32_t Snk_PDO4, uint32_t Snk_PDO5, uint32_t Snk_PDO6){

	Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, SNK_PDO_SET);
	uint8_t command_write_data_0 = 0b10000111 & ((mry_write << 7) | No_of_PDOs);
	Register_Write(I2C_MASTER, COMMAND_WRITE_DATA_0,command_write_data_0) ;
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_1,  Snk_PDO1);
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_5,  Snk_PDO2);
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_9,  Snk_PDO3);
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_13, Snk_PDO4);
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_17, Snk_PDO5);
	Multi_Register_4byte_Write(I2C_MASTER, COMMAND_WRITE_DATA_21, Snk_PDO6);

	Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);

}

void Sink_PDO_Req(SNK_PDO_union *comb_snk_PDO_req, uint8_t* No_of_PDOs, uint32_t* SNK_PDO1, uint32_t* SNK_PDO2, uint32_t* SNK_PDO3, uint32_t* SNK_PDO4, uint32_t* SNK_PDO5, uint32_t* SNK_PDO6){

	uint8_t NO_OF_READ_ADDRESS = 36;
	Register_Write(I2C_MASTER, COMMAND_WRITE_ADDRESS, SNK_PDO_REQUEST_READ);
	Register_Write(I2C_MASTER, COMMAND_END_ADDRESS, 0x00);
	Register_Multi_Read(I2C_MASTER, COMMAND_READ_ADDRESS, NO_OF_READ_ADDRESS, comb_snk_PDO_req->SNK_PDO_Combined);
    *No_of_PDOs = comb_snk_PDO_req->snk_pdo.No_of_PDOs;
	*SNK_PDO1 = comb_snk_PDO_req->snk_pdo.PDO1_1 | comb_snk_PDO_req->snk_pdo.PDO1_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO1_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO1_4 << 24 ;
	*SNK_PDO2 = comb_snk_PDO_req->snk_pdo.PDO2_1 | comb_snk_PDO_req->snk_pdo.PDO2_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO2_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO2_4 << 24 ;
	*SNK_PDO3 = comb_snk_PDO_req->snk_pdo.PDO3_1 | comb_snk_PDO_req->snk_pdo.PDO3_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO3_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO3_4 << 24 ;
	*SNK_PDO4 = comb_snk_PDO_req->snk_pdo.PDO4_1 | comb_snk_PDO_req->snk_pdo.PDO4_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO4_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO4_4 << 24 ;
	*SNK_PDO5 = comb_snk_PDO_req->snk_pdo.PDO5_1 | comb_snk_PDO_req->snk_pdo.PDO5_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO5_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO5_4 << 24 ;
	*SNK_PDO6 = comb_snk_PDO_req->snk_pdo.PDO6_1 | comb_snk_PDO_req->snk_pdo.PDO6_2 << 8 | comb_snk_PDO_req->snk_pdo.PDO6_3 << 16 | comb_snk_PDO_req->snk_pdo.PDO6_4 << 24 ;


}

void Set_Interrupt_Mask(){
  Register_Write(I2C_MASTER, UIC_INT_M, 0b10000000);
}

void gpio_isr(void *cbdata)
{
	uint8_t UIC_Int_Stat[1];
	//printf("Interrupt Status after: %x\n",UIC_Stat);
	Register_Read(I2C_MASTER, UIC_INT, UIC_Int_Stat);
	uint8_t UIC_Stat = UIC_Int_Stat[0];
	printf("Interrupt Status: %x\n",UIC_Stat);
    mxc_gpio_cfg_t *cfg = cbdata;

    uint16_t Device_ID;
    uint8_t dev_id[1];

    Register_Read(I2C_MASTER, DEVICE_ID, dev_id);
    Device_ID = 0xFF & (dev_id[0]);
    printf("Device_ID = %x\n", Device_ID);

//    MXC_Delay(1000000);




}

void GPIO_Interrupt_Enable(){
    mxc_gpio_cfg_t gpio_interrupt;
	mxc_gpio_cfg_t gpio_interrupt_status;

	/* Setup interrupt status pin as an output so we can toggle it on each interrupt. */
	gpio_interrupt_status.port = LED_PORT;
	gpio_interrupt_status.mask = LED_PIN;
	gpio_interrupt_status.pad = MXC_GPIO_PAD_NONE;
	gpio_interrupt_status.func = MXC_GPIO_FUNC_OUT;
	gpio_interrupt_status.vssel = MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&gpio_interrupt_status);
	/*
	 *   Set up interrupt the gpio.
	 *   Switch on EV kit is open when non-pressed, and grounded when pressed.
	 *   Use an internal pull-up so pin reads high when button is not pressed.
	 */
	gpio_interrupt.port = BUTTON_PORT;
	gpio_interrupt.mask = BUTTON_PIN;
	gpio_interrupt.pad = MXC_GPIO_PAD_PULL_UP;
	gpio_interrupt.func = MXC_GPIO_FUNC_IN;
	gpio_interrupt.vssel = MXC_GPIO_VSSEL_VDDIOH;
	MXC_GPIO_Config(&gpio_interrupt);
	MXC_GPIO_RegisterCallback(&gpio_interrupt, gpio_isr, &gpio_interrupt_status);
	MXC_GPIO_IntConfig(&gpio_interrupt, MXC_GPIO_INT_FALLING);
	MXC_GPIO_EnableInt(gpio_interrupt.port, gpio_interrupt.mask);
	NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(BUTTON_PORT)));

}


void VBUS_ADC_Voltage_print(uint8_t* Voltage_int){
	switch(*Voltage_int)
	{
	case 0x00: printf("VBUS: 0x00 = VBUS < 3.5V\n");
	break;

	case 0x01: printf("VBUS: 0x01 = 3.5V < VBUS < 4.5V\n");
	break;

	case 0x02: printf("VBUS: 0x02 = 4.5V < VBUS < 5.5V\n");
	break;

	case 0x03: printf("VBUS: 0x03 = 5.5V < VBUS < 6.5V\n");
	break;

	case 0x04: printf("VBUS: 0x04 = 6.5V < VBUS < 7.5V\n");
	break;

	case 0x05: printf("VBUS: 0x05 = 7.5V < VBUS < 8.5V\n");
	break;

	case 0x06: printf("VBUS: 0x06 = 8.5V < VBUS < 9.5V\n");
	break;

	case 0x07: printf("VBUS: 0x07 = 9.5V < VBUS < 10.5V\n");
	break;

	case 0x08: printf("VBUS: 0x08 = 10.5V < VBUS < 11.5V\n");
	break;

	case 0x09: printf("VBUS: 0x09 = 11.5V < VBUS < 12.5V\n");
	break;

	case 0x0A: printf("VBUS: 0x0A = 12.5V < VBUS < 13.5V\n");
	break;

	case 0x0B: printf("VBUS: 0x0B = 13.5V < VBUS < 14.5V\n");
	break;

	case 0x0C: printf("VBUS: 0x0C = 14.5V < VBUS < 15.5V\n");
	break;

	case 0x0D: printf("VBUS: 0x0D = 15.5V < VBUS < 16.5V\n");
	break;

	case 0x0E: printf("VBUS: 0x0E = 16.5V < VBUS < 17.5V\n");
	break;

	case 0x0F: printf("VBUS: 0x0F = 17.5V < VBUS < 18.5V\n");
	break;

	case 0x10: printf("VBUS: 0x10 = 18.5V < VBUS < 19.5V\n");
	break;

	case 0x11: printf("VBUS: 0x11 = 19.5V < VBUS < 20.5V\n");
	break;

	case 0x12: printf("VBUS: 0x12 = 19.5V < VBUS < 20.5V\n");
	break;

	case 0x13: printf("VBUS: 0x13 = 21.5V < VBUS < 22.5V\n");
	break;

	case 0x14: printf("VBUS: 0x14 = 22.5V < VBUS < 23.5V\n");
	break;

	case 0x15: printf("VBUS: 0x15 = 23.5V < VBUS < 24.5V\n");
	break;

	case 0x16: printf("VBUS: 0x16 = 24.5V < VBUS < 25.5V \n");
	break;

	case 0x17: printf("VBUS: 0x17 = 25.5V < VBUS < 26.5V\n");
	break;

	case 0x18: printf("VBUS: 0x18 = 26.5V < VBUS < 27.5V\n");
	break;

	case 0x19: printf("VBUS: 0x19 = 27.5V < VBUS\n");
	break;

	case 0x1A: printf("VBUS: 0x1A = Reserved\n");
	break;

	default: printf("Unknown VBUS");
	break;
	}
}

void Port_current_print(uint8_t* Current_Int){
	switch(*Current_Int){

	case 0x00: printf("CC detected current limit: Not in UFP mode\n");
	break;

	case 0x01: printf("CC detected current limit: 500mA\n");
	break;

	case 0x02: printf("CC detected current limit: 1500mA\n");
	break;

	case 0x03: printf("CC detected current limit: 3000mA\n");
	break;

	default: printf("Unknown");
	break;
	}
}

void PD_Message_Print(uint8_t* PD_Mssg){
	switch(*PD_Mssg)
	{
	case 0x00: printf("PD Message: Nothing happened\n");
	break;

	case 0x01: printf("PD Message: Sink_PD_PSRdy_Received\n");
	break;

	case 0x02: printf("PD Message: Sink_PD_Error_Recovery\n");
	break;

	case 0x03: printf("PD Message: Sink_PD_SenderResponseTimer_Timeout\n");
	break;

	case 0x04: printf("PD Message: Source_PSRdy_Sent\n");
	break;

	case 0x05: printf("PD Message: Source_PD_Error_Recovery\n");
	break;

	case 0x06: printf("PD Message: Source_PD_SenderResponseTimer_Timeout\n");
	break;

	case 0x07: printf("PD Message: PD_DR_Swap_Request_Received\n");
	break;

	case 0x08: printf("PD Message: PD_PR_Swap_Request_Received\n");
	break;

	case 0x09: printf("PD Message: PD_VCONN_Swap_Request_Received\n");
	break;

	case 0x0A: printf("PD Message: Received PD Message in illegal state\n");
	break;

	case 0x0B: printf("PD Message: Sink_PD_Evaluate_State, SrcCap_Received\n");
	break;

	case 0x11: printf("PD Message: VDM Attention Message Received\n");
	break;

	case 0x12: printf("PD Message: Reject_Received\n");
	break;

	case 0x13: printf("PD Message: Not_Supported_Received\n");
	break;

	case 0x14: printf("PD_PR_Swap_SNKTOSRC_Cleanup\n");
	break;

	case 0x15: printf("PD Message: PD_PR_Swap_SRCTOSNK_Cleanup\n");
	break;

	case 0x16: printf("PD Message: HardReset_Received\n");
	break;

	case 0x17: printf("PD Message: PD_PowerSupply_VbusEnable\n");
	break;

	case 0x18: printf("PD Message: PD_PowerSupply_VbusDisable\n");
	break;

	case 0x19: printf("PD Message: HardReset_Sent\n");
	break;

	case 0x1A: printf("PD Message: PD_PR_Swap_SRCTOSWAP\n");
	break;

	case 0x1B: printf("PD Message: PD_PR_Swap_SWAPTOSNK\n");
	break;

	case 0x1C: printf("PD Message: PD_PR_Swap_SNKTOSWAP\n");
	break;

	case 0x1D: printf("PD_PR_Swap_SWAPTOSRC\n");
	break;

	case 0x20: printf("PD Message: Sink_PD_Disabled\n");
	break;

	case 0x21: printf("PD Message: Source_PD_Disabled\n");
	break;

	case 0x30: printf("PD Message: Get_Source_Capabilities_Extended_Received\n");
	break;

	case 0x31: printf("PD Message: Get_Status_Received\n");
	break;

	case 0x32: printf("PD Message: Get_Battery_Cap_Received\n");
	break;

	case 0x33: printf("PD Message: Get_Battery_Status_Received\n");
	break;

	case 0x34: printf("PD Message: Get_Manufacturer_Info_Received\n");
	break;

	case 0x35: printf("PD Message: Source_Capabilities_Extended_Received\n");
	break;

	case 0x36: printf("PD Message: Status_Received\n");
	break;

	case 0x37: printf("PD Message: Battery_Capabilities_Received\n");
	break;

	case 0x38: printf("PD Message: Battery_Status_Received\n");
	break;

	case 0x39: printf("PD Message: Manufacturer_Info_Received\n");
	break;

	case 0x3A: printf("PD Message: Security_Request_Received\n");
	break;

	case 0x3B: printf("PD Message: Security_Response_Received\n");
	break;

	case 0x3C: printf("PD Message: Firmware_Update_Request_Received\n");
	break;

	case 0x3D: printf("PD Message: Firmware_Update_Response_Received\n");
	break;

	case 0x3E: printf("PD Message: Alert_Received\n");
	break;

	case 0x40: printf("PD Message: VDM_NAK_Received\n");
	break;

	case 0x41: printf("PD Message: VDM_BUSY_Received\n");
	break;

	case 0x42: printf("PD Message: VDM_ACK_Received\n");
	break;

	case 0x43: printf("PD Message: VDM_REQ_Received\n");
	break;

	case 0x63: printf("PD Message: DiscoverMode_Received\n");
	break;

	case 0x65: printf("PD Message: PD_Status_Received\n");
	break;

	default: printf("Unknown\n");
	break;

	}
}



void Port_Detection_Status_Voltage_current_Read(uint8_t* Voltage_Read, uint8_t* Current_Read, uint8_t* PD_Status0){
	uint8_t BC_STATUS_return[1];
	Port_type Charger_type;
	uint8_t CC_Status1_ret[1];
	uint8_t PD_Status0_ret[1];
	uint8_t PD_Status1_ret[1];


	Register_Read(I2C_MASTER, BC_STATUS, BC_STATUS_return);


	Charger_type = (0b00000011 & BC_STATUS_return[0]);

	switch(Charger_type){

	case Nothing_Attached:
		 printf("Port type: Nothing Attached\n");
		 Register_Read(I2C_MASTER, USBC_STATUS1, USB_STATUS_1_Read);
		 Register_Read(I2C_MASTER, CC_STATUS0, CC_Istat);
		 *Voltage_Read = 0b00011111 & (USB_STATUS_1_Read[0] >> 3);
		 *Current_Read = 0b00000011 & (CC_Istat[0] >> 4);
		 break;

	case SDP:
		 printf("Port type: SDP\n");
		 Register_Read(I2C_MASTER, USBC_STATUS1, USB_STATUS_1_Read);
		 Register_Read(I2C_MASTER, CC_STATUS0, CC_Istat);
		 *Voltage_Read = 0b00011111 & (USB_STATUS_1_Read[0] >> 3);
		 *Current_Read = 0b00000011 & (CC_Istat[0] >> 4);
		 break;

	case CDP:
		printf("Port type: CDP\n");
		Register_Read(I2C_MASTER, USBC_STATUS1, USB_STATUS_1_Read);
		Register_Read(I2C_MASTER, CC_STATUS0, CC_Istat);
		*Voltage_Read = 0b00011111 & (USB_STATUS_1_Read[0] >> 3);
		*Current_Read = 0b00000011 & (CC_Istat[0] >> 4);
		break;

	case DCP:
		printf("Port type: DCP\n");
		Register_Read(I2C_MASTER, USBC_STATUS1, USB_STATUS_1_Read);
		Register_Read(I2C_MASTER, CC_STATUS0, CC_Istat);
		*Voltage_Read = 0b00011111 & (USB_STATUS_1_Read[0] >> 3);
		*Current_Read = 0b00000011 & (CC_Istat[0] >> 4);
        break;

	default:
		printf("Port Error\n");
		break;
	}

	VBUS_ADC_Voltage_print(Voltage_Read);
	Port_current_print(Current_Read);

	uint8_t VBUS_det = 0b00000001 & (BC_STATUS_return[0] >> 7);

	switch(VBUS_det)
	{

	case 0x0: printf("VBUS < VBDET\n");
	break;

	case 0x1: printf("VBUS > VBDET\n");
    break;

	default: printf("Unknown");
	break;
	}

	uint8_t CC_pin_state_mac = 0b00000111 & (CC_Istat[0]);

	switch(CC_pin_state_mac)
	{
	case 0x00: printf("CC_pin_machine: No connection\n");
	break;

	case 0x01: printf("CC_pin_machine: SINK\n");
    break;

	case 0x02: printf("CC_pin_machine: SOURCE\n");
	break;

	case 0x03: printf("CC_pin_machine: Audio accessory\n");
	break;

	case 0x04: printf("CC_pin_machine: DebugSrc accessory\n");
	break;

	case 0x05: printf("CC_pin_machine: Error\n");
	break;

	case 0x06: printf("CC_pin_machine: Disabled\n");
	break;

	case 0x07: printf("CC_pin_machine: DebugSnk accessory\n");
	break;

	default: printf("Unknown\n");
    break;
	}

	uint8_t CC_pin_stat = 0b00000011 & (CC_Istat[0] >> 6);

	switch(CC_pin_stat)
	{
	case 0x00: printf("No determination\n");
	break;

	case 0x01: printf("Port orientation: CC1 Active\n");
	break;

	case 0x02: printf("Port orientation: CC2 Active\n");
	break;

	case 0x03: printf("Port orientation: RFU\n");
	break;

	default: printf("Unknown\n");
	break;
	}

	Register_Read(I2C_MASTER, CC_STATUS1, CC_Status1_ret);
	uint8_t VSAFE_OV = 0b00000001 & (CC_Status1_ret[0] >> 3);

	switch(VSAFE_OV)
	{
	case 0x00: printf("VBUS < VSAFE0V\n");
	break;

	case 0x01: printf("VBUS > VSAFE0V\n");
	break;

	default: printf("Unknown\n");
	break;
	}


	Register_Read(I2C_MASTER, PD_STATUS0, PD_Status0_ret);
    *PD_Status0 = PD_Status0_ret[0];
    PD_Message_Print(PD_Status0);


    Register_Read(I2C_MASTER, PD_STATUS1, PD_Status1_ret);
    uint8_t PSRDY = 0b00000001 & (PD_Status1_ret[0] >> 4);

    switch(PSRDY)
    {
    case 0x00: printf("Nothing happened\n");
    break;

    case 0x01: printf("PSRDY received\n");
    break;

    default: printf("Unknown\n");
    break;
    }

    uint8_t Power_Role = 0b00000001 & (PD_Status1_ret[0] >> 6);

    switch(Power_Role)
    {
    case 0x00: printf("Power role: Sink\n");
    break;

    case 0x01: printf("Power role: Source\n");
    break;

    default: printf("Unknown\n");
    break;
    }

    uint8_t Data_Role = 0b00000001 & (PD_Status1_ret[0] >> 7);

    switch(Data_Role)
    {
    case 0x00: printf("Data Role: UFP\n");
    break;

    case 0x01: printf("Data Role: DFP\n");
    break;

    default: printf("Unknown\n");
    break;
    }



}
void Register_Write_Charger(uint8_t reg_addr, uint8_t Write_data, uint8_t Len)
{
    Register_Write(I2C_MASTER,COMMAND_WRITE_ADDRESS,MASTER_I2C_WRITE);
    Register_Write(I2C_MASTER,COMMAND_WRITE_DATA_0,SLAVE_WRITE_ADDRESS);
    Register_Write(I2C_MASTER,COMMAND_WRITE_DATA_1,reg_addr);
    Register_Write(I2C_MASTER,COMMAND_WRITE_DATA_2,Len);
    Register_Write(I2C_MASTER,COMMAND_WRITE_DATA_3,Write_data);
    Register_Write(I2C_MASTER,COMMAND_END_ADDRESS,0x00);

}


void MAX77962_Charger_initial_settings(uint8_t Term_Voltage, uint8_t Curr_input_limit, uint8_t Fast_chg_curr_LSB, uint8_t Fast_chg_curr_MSB)
{
   uint8_t Config_reg_4 = 0xFF & (Fast_chg_curr_MSB << 7 |Term_Voltage);
   Register_Write_Charger(CHG_CNFG_00,0x85,1);
   Register_Write_Charger(CHG_CNFG_04,Config_reg_4,1);
   Register_Write_Charger(CHG_CNFG_08,Curr_input_limit,1);
   Register_Write_Charger(CHG_CNFG_02, Fast_chg_curr_LSB,1);

}











#endif /* FUNCTIONS_H_ */
