#pragma once

#include <stdio.h>
#include <stdint.h>


#include "MAX20360_fields.h"
#include "MAX20360_Registers.h"

#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2c.h"
#include "dma.h"




#define VOLTAGE_OUT_OF_RANGE           0xFF


//Declaring I2C master instance

mxc_i2c_req_t reqMaster;


//Defines register bitfields


typedef struct {

	unsigned int Enable : 2;
	unsigned int        : 3;
	unsigned int Seq    : 3;

}Enable_Config;

typedef struct{

    unsigned int LDO_passive_dis_ctrl : 1;
    unsigned int LDO_Mode             : 1;
    unsigned int LDO_active_dis_ctrl  : 1;
    unsigned int MPC0_ctrl_bit        : 1;
    unsigned int MPC0_config          : 1;
    unsigned int                      : 3;

}LDO_1_Conf;

typedef struct{

	unsigned int LDO_passive_dis_ctrl : 1;
	unsigned int LDO_Mode             : 1;
	unsigned int LDO_active_dis_ctrl  : 1;
	unsigned int LDO_internal_supply  : 1;
	unsigned int                      : 4;

}LDO_2_Conf;


typedef struct{

    unsigned int MPC_0 : 1;
    unsigned int MPC_1 : 1;
    unsigned int MPC_2 : 1;
    unsigned int MPC_3 : 1;
    unsigned int MPC_4 : 1;
    unsigned int MPC_5 : 1;
    unsigned int MPC_6 : 1;
    unsigned int MPC_7 : 1;

} Control_Config;

typedef struct{

	unsigned int BuckEnLXSns  : 1;
	unsigned int BuckFETScale : 1;
	unsigned int BuckLowEMI   : 1;
	unsigned int BuckActDsc   : 1;
	unsigned int BuckPsvDsc   : 1;
	unsigned int BuckFast     : 1;
	unsigned int BuckPGOODEn  : 1;
	unsigned int BuckIntegDis : 1;

}Buck_Cfg0;

typedef struct{

	unsigned int               : 3;
	unsigned int BuckIAdptDis  : 1;
	unsigned int BuckFPWM      : 1;
	unsigned int BuckMPC2Fast  : 1;
	unsigned int Buck3DisLDO   : 1;
	unsigned int               : 1;

}Buck_Cfg1;

typedef struct {

	unsigned int BBstPsvDisc      : 1;
	unsigned int BBstMode         : 1;
	unsigned int BBstRampEn       : 1;
	unsigned int BBstActDsc       : 1;
	unsigned int BBstLowEMI       : 1;
	unsigned int                  : 2;
	unsigned int BBstISetLookUpDis: 1;

}BBst_Cfg0;

typedef struct{

	unsigned int BBFHighSh        : 2;
	unsigned int BBstMPC1FastCntl : 1;
	unsigned int BBstFETScale     : 1;
	unsigned int BBstZCCmpDis     : 1;
	unsigned int BBstFast         : 1;
	unsigned int BBstIAdptDis     : 1;
	unsigned int                  : 1;

}BBst_Cfg1;

typedef struct {

	unsigned int BstFETScale : 1;
	unsigned int BstFastStrt : 1;
	unsigned int BstIAdptEn  : 1;
	unsigned int BstPsvDsc   : 1;

}Boost_Cfg;




typedef union
{
  Enable_Config enable_conf;
  uint8_t enable_combined;

}Combined_bit_fields_Enable_Config;


typedef union{

    LDO_1_Conf LDO_1_conf;
    uint8_t LDO_1_conf_combined;

}Combined_bit_fields_LDO_1_conf;

typedef union{
	LDO_2_Conf LDO_2_conf;
	uint8_t LDO_2_conf_combined;

}Combined_bit_fields_LDO_2_conf;

typedef union{

	Buck_Cfg0 buck_cfg0;
	uint8_t buck_cfg0_combined;

}Combined_bit_fields_Buck_Cfg0;

typedef union{

	Buck_Cfg1 buck_cfg1;
	uint8_t buck_cfg1_combined;

}Combined_bit_fields_Buck_Cfg1;

typedef union
{
  Control_Config Conv_control;
  uint8_t control_combined;

}Combined_bit_fields_Control_Config;

typedef union{

	BBst_Cfg0 bbst_cfg0;
	uint8_t bbst_cfg0_combined;

}Combined_bit_fields_BBst_Cfg0;

typedef union{

	BBst_Cfg1 bbst_cfg1;
	uint8_t bbst_cfg1_combined;

}Combined_bit_fields_BBst_Cfg1;

typedef union{

	Boost_Cfg boost_cfg;
	uint8_t boost_cfg_combined;

}Combined_bit_fields_Boost_Cfg;











//Functions Declarations

void Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg, uint8_t* Read_value);
void Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg_addr, uint8_t Single_Byte_Write_Value);
uint8_t LDO_1_Set_Voltage(uint16_t Voltage_in_mv);
uint8_t LDO_2_Set_Voltage(uint16_t Voltage_in_mv);
void Lock(Lock_Selection selection, uint8_t lock_unlock);
void LDO_configure(LDO_selection LDO_index, State enable, Combined_bit_fields_Enable_Config *comb_LDO_en, Combined_bit_fields_LDO_1_conf *comb_LDO_1_conf,Combined_bit_fields_LDO_2_conf *comb_LDO_2_conf);




//Function definitions

void Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg, uint8_t* Read_value)
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = slave_addr;
	reqMaster.tx_buf = &reg;                                     //tx_buf is a pointer
	reqMaster.tx_len = sizeof(reg);
	reqMaster.rx_buf = Read_value;                               //
	reqMaster.rx_len = 2;                                        //rx_len !=0 fir read operation
	MXC_I2C_MasterTransaction(&reqMaster);

}


void Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg_addr, uint8_t Single_Byte_Write_Value) //
{
	//CRC check
	uint8_t CRC1 = (uint8_t)(((0x28 << 1) + reg_addr + Single_Byte_Write_Value) % 255);
	uint8_t CRC2 = (uint8_t)((3*(0x28 << 1) + 2*reg_addr + Single_Byte_Write_Value) % 255);

	reqMaster.i2c = i2c_master;
	reqMaster.addr = slave_addr;
	uint8_t buf[4] = {reg_addr, Single_Byte_Write_Value, CRC1, CRC2}; // is write data is more than 16bit, add Write_value1_LSB, Write_value1_MSB and so on...
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_len = 0;                                        //rx_len = 0 for write operation
	MXC_I2C_MasterTransaction(&reqMaster);
}

void Register_Read_FG(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg, uint8_t* Read_value_FG)
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = slave_addr;
	reqMaster.tx_buf = &reg;                                     //tx_buf is a pointer
	reqMaster.tx_len = sizeof(reg);
	reqMaster.rx_buf = Read_value_FG;
	reqMaster.rx_len = 2;                                        //rx_len !=0 fir read operation
	MXC_I2C_MasterTransaction(&reqMaster);

}



void Register_Write_FG(mxc_i2c_regs_t* i2c_master, uint8_t slave_addr, uint8_t reg_addr, uint8_t Write_Value_LSB, uint8_t Write_Value_MSB) //
{

	reqMaster.i2c = i2c_master;
	reqMaster.addr = slave_addr;
	uint8_t buf[3] = {reg_addr, Write_Value_LSB, Write_Value_MSB}; // is write data is more than 16bit, add Write_value1_LSB, Write_value1_MSB and so on...
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_len = 0;                                        //rx_len = 0 for write operation
	MXC_I2C_MasterTransaction(&reqMaster);
}



uint8_t LDO_1_Set_Voltage(uint16_t Voltage_in_mv) {
    // Check if the input value is within the specified range
    if (Voltage_in_mv < 500 || Voltage_in_mv > 1950) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
        return VOLTAGE_OUT_OF_RANGE;
    }

    // Calculate the corresponding binary value
    uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 500) / 25);

    Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO1VSET, voltage_in_binary);

    return voltage_in_binary;
}




uint8_t LDO_2_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 900 || Voltage_in_mv > 4000) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 900) / 100);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO2VSET, voltage_in_binary);

	return voltage_in_binary;
}




void LDO_configure(LDO_selection LDO_index, State enable, Combined_bit_fields_Enable_Config *comb_LDO_en, Combined_bit_fields_LDO_1_conf *comb_LDO_1_conf,Combined_bit_fields_LDO_2_conf *comb_LDO_2_conf)
{
    switch (enable)
    {
    case ENABLED:
    	comb_LDO_en->enable_conf.Enable =  (unsigned int) ENABLED;
        break;

    case DISABLED:
    	comb_LDO_en->enable_conf.Enable =  (unsigned int) DISABLED;
        break;

    case MPC_CONTROL:
    	comb_LDO_en->enable_conf.Enable =  (unsigned int) MPC_CONTROL;
        break;

    default:
        printf("LDO unknown state");
        break;
    }

    if (LDO_index == LDO_1)       // enabling and configuring LDO 1
    {
    	comb_LDO_1_conf->LDO_1_conf.LDO_passive_dis_ctrl = 1;
    	comb_LDO_1_conf->LDO_1_conf.LDO_Mode             = 0;
    	comb_LDO_1_conf->LDO_1_conf.LDO_active_dis_ctrl  = 0;
    	comb_LDO_1_conf->LDO_1_conf.MPC0_ctrl_bit        = 0;
    	comb_LDO_1_conf->LDO_1_conf.MPC0_config          = 0;
        Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO1ENA, comb_LDO_en->enable_combined);
        Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO1CFG, comb_LDO_1_conf->LDO_1_conf_combined);
    }
    else if (LDO_index == LDO_2){                               // enabling and configuring LDO_2
    	comb_LDO_2_conf->LDO_2_conf.LDO_passive_dis_ctrl = 1;
    	comb_LDO_2_conf->LDO_2_conf.LDO_Mode             = 0;
    	comb_LDO_2_conf->LDO_2_conf.LDO_active_dis_ctrl  = 0;
    	comb_LDO_2_conf->LDO_2_conf.LDO_internal_supply  = 1;
        Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO2ENA, comb_LDO_en->enable_combined);
        Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LDO2CFG, comb_LDO_2_conf->LDO_2_conf_combined);
    }

    else{
    	printf("LDO not selected\n");
    }
}



uint8_t Buck1_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 550 || Voltage_in_mv > (550 + 63*BUCK1STEP_MAX20360J)) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 550) / BUCK1STEP_MAX20360J);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK1VSET, voltage_in_binary);

	return voltage_in_binary;
}


void Buck1_configure(State enable, Combined_bit_fields_Enable_Config *comb_Buck_1_en, Combined_bit_fields_Buck_Cfg0 *comb_Buck_1_conf0,Combined_bit_fields_Buck_Cfg1 *comb_Buck_1_conf1)
{
	switch (enable)
	{
	case ENABLED:
		comb_Buck_1_en->enable_conf.Enable =  (unsigned int) ENABLED;
		break;

	case DISABLED:
		comb_Buck_1_en->enable_conf.Enable =  (unsigned int) DISABLED;
		break;

	case MPC_CONTROL:
		comb_Buck_1_en->enable_conf.Enable =  (unsigned int) MPC_CONTROL;
		break;

	default:
		printf("Buck unknown state");
		break;
	}
    //Configuration 0
	comb_Buck_1_conf0->buck_cfg0.BuckEnLXSns  = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckFETScale = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckLowEMI   = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckActDsc   = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckPsvDsc   = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckFast     = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckPGOODEn  = 0;
	comb_Buck_1_conf0->buck_cfg0.BuckIntegDis = 0;

	//Configuration1

	comb_Buck_1_conf1->buck_cfg1.BuckIAdptDis = 0;
	comb_Buck_1_conf1->buck_cfg1.BuckFPWM     = 0;
	comb_Buck_1_conf1->buck_cfg1.BuckMPC2Fast = 0;

	//Writing via I2C

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK1ENA, comb_Buck_1_en->enable_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK1CFG0, comb_Buck_1_conf0->buck_cfg0_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK1CFG1, comb_Buck_1_conf1->buck_cfg1_combined);
//	    uint8_t LDO_2_enable[1];
//	    Register_Read(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK1ENA, LDO_2_enable);
//	    uint8_t LDO_2_enabled = LDO_2_enable[0];
//	    printf("Buck return: %x \n", LDO_2_enabled);
}


uint8_t Buck2_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 550 || Voltage_in_mv > (550 + 63*BUCK2STEP_MAX20360J)) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 550) / BUCK2STEP_MAX20360J);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK2VSET, voltage_in_binary);

	return voltage_in_binary;
}



void Buck2_configure(State enable, Combined_bit_fields_Enable_Config *comb_Buck_2_en, Combined_bit_fields_Buck_Cfg0 *comb_Buck_2_conf0,Combined_bit_fields_Buck_Cfg1 *comb_Buck_2_conf1)
{
	switch (enable)
	{
	case ENABLED:
		comb_Buck_2_en->enable_conf.Enable =  (unsigned int) ENABLED;
		break;

	case DISABLED:
		comb_Buck_2_en->enable_conf.Enable =  (unsigned int) DISABLED;
		break;

	case MPC_CONTROL:
		comb_Buck_2_en->enable_conf.Enable =  (unsigned int) MPC_CONTROL;
		break;

	default:
		printf("Buck unknown state");
		break;
	}
    //Configuration 0
	comb_Buck_2_conf0->buck_cfg0.BuckEnLXSns  = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckFETScale = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckLowEMI   = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckActDsc   = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckPsvDsc   = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckFast     = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckPGOODEn  = 0;
	comb_Buck_2_conf0->buck_cfg0.BuckIntegDis = 0;

	//Configuration1

	comb_Buck_2_conf1->buck_cfg1.BuckIAdptDis = 0;
	comb_Buck_2_conf1->buck_cfg1.BuckFPWM = 0;
	comb_Buck_2_conf1->buck_cfg1.BuckMPC2Fast = 0;

	//Writing via I2C

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK2ENA, comb_Buck_2_en->enable_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK2CFG, comb_Buck_2_conf0->buck_cfg0_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK2CFG1, comb_Buck_2_conf1->buck_cfg1_combined);

}

uint8_t Buck3_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 550 || Voltage_in_mv > (550 + 63*BUCK3STEP)) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 550) / BUCK3STEP);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK3VSET, voltage_in_binary);

	return voltage_in_binary;
}

void Buck3_configure(State enable, Combined_bit_fields_Enable_Config *comb_Buck_3_en, Combined_bit_fields_Buck_Cfg0 *comb_Buck_3_conf0,Combined_bit_fields_Buck_Cfg1 *comb_Buck_3_conf1)
{
	switch (enable)
	{
	case ENABLED:
		comb_Buck_3_en->enable_conf.Enable =  (unsigned int) ENABLED;
		break;

	case DISABLED:
		comb_Buck_3_en->enable_conf.Enable =  (unsigned int) DISABLED;
		break;

	case MPC_CONTROL:
		comb_Buck_3_en->enable_conf.Enable =  (unsigned int) MPC_CONTROL;
		break;

	default:
		printf("Buck unknown state");
		break;
	}
    //Configuration 0
	comb_Buck_3_conf0->buck_cfg0.BuckEnLXSns  = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckFETScale = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckLowEMI   = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckActDsc   = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckPsvDsc   = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckFast     = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckPGOODEn  = 0;
	comb_Buck_3_conf0->buck_cfg0.BuckIntegDis = 0;

	//Configuration1

	comb_Buck_3_conf1->buck_cfg1.Buck3DisLDO  = 0;
	comb_Buck_3_conf1->buck_cfg1.BuckIAdptDis = 0;
	comb_Buck_3_conf1->buck_cfg1.BuckFPWM     = 0;
	comb_Buck_3_conf1->buck_cfg1.BuckMPC2Fast = 0;

	//Writing via I2C

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK3ENA, comb_Buck_3_en->enable_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK3CFG, comb_Buck_3_conf0->buck_cfg0_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BUCK3CFG1, comb_Buck_3_conf1->buck_cfg1_combined);

}

uint8_t Boost_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 5000 || Voltage_in_mv > 20000) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 5000) / BOOSTSTEP);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BOOSTVSET, voltage_in_binary);

	return voltage_in_binary;
}

void Boost_configure(State enable, Combined_bit_fields_Enable_Config *comb_Boost_en, Combined_bit_fields_Boost_Cfg *comb_Boost_conf){

	switch(enable)
	{
	case DISABLED: comb_Boost_en->enable_conf.Enable = DISABLED;
	break;

	case ENABLED: comb_Boost_en->enable_conf.Enable = ENABLED;
	break;

	case MPC_CONTROL: comb_Boost_en->enable_conf.Enable = MPC_CONTROL;
	break;

	default:printf("Boost unknown state");
	break;
	}

	//Boost Configuration

	comb_Boost_conf->boost_cfg.BstFETScale = 0;
	comb_Boost_conf->boost_cfg.BstFastStrt = 0;
	comb_Boost_conf->boost_cfg.BstIAdptEn  = 1;
	comb_Boost_conf->boost_cfg.BstPsvDsc   = 1;

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BOOSTENA, comb_Boost_en->enable_combined);
    Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BOOSTCFG, comb_Boost_conf->boost_cfg_combined);

}

uint8_t BB_Set_Voltage(uint16_t Voltage_in_mv) {
	// Check if the input value is within the specified range
	if (Voltage_in_mv < 2500 || Voltage_in_mv > 5500) {  //note: By default 0.5 and 1.95 are floats but the input voltage in volt is double so we have to cast constants to double
		return VOLTAGE_OUT_OF_RANGE;
	}

	// Calculate the corresponding binary value
	uint8_t voltage_in_binary = (uint8_t)((Voltage_in_mv - 2500) / BBSTEP);

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BBSTVSET, voltage_in_binary);

	return voltage_in_binary;
}

void BB_configure(State enable, Combined_bit_fields_Enable_Config *comb_Bbst_en, Combined_bit_fields_BBst_Cfg0 *comb_BBst_cfg0, Combined_bit_fields_BBst_Cfg1 *comb_BBst_cfg1)
{
	switch(enable)
		{
		case DISABLED: comb_Bbst_en->enable_conf.Enable = DISABLED;
		break;

		case ENABLED: comb_Bbst_en->enable_conf.Enable = ENABLED;
		break;

		case MPC_CONTROL: comb_Bbst_en->enable_conf.Enable = MPC_CONTROL;
		break;

		default:printf("Buck-Boost unknown state");
		break;
		}

	comb_BBst_cfg0->bbst_cfg0.BBstPsvDisc       = 1;
	comb_BBst_cfg0->bbst_cfg0.BBstMode          = 0;  //"1" for buck only mode
	comb_BBst_cfg0->bbst_cfg0.BBstRampEn        = 1;
	comb_BBst_cfg0->bbst_cfg0.BBstActDsc        = 0;
	comb_BBst_cfg0->bbst_cfg0.BBstLowEMI        = 0;
	comb_BBst_cfg0->bbst_cfg0.BBstISetLookUpDis = 0;

	comb_BBst_cfg1->bbst_cfg1.BBFHighSh         = 0b11;
	comb_BBst_cfg1->bbst_cfg1.BBstMPC1FastCntl  = 0;
	comb_BBst_cfg1->bbst_cfg1.BBstFETScale      = 0;
	comb_BBst_cfg1->bbst_cfg1.BBstZCCmpDis      = 0;
	comb_BBst_cfg1->bbst_cfg1.BBstFast          = 0;
	comb_BBst_cfg1->bbst_cfg1.BBstIAdptDis      = 1;

	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BBSTENA, comb_Bbst_en->enable_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BBSTCFG, comb_BBst_cfg0->bbst_cfg0_combined);
	Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, BBSTCFG1,comb_BBst_cfg1->bbst_cfg1_combined);
}

void Lock(Lock_Selection selection, uint8_t lock_unlock){

	switch(selection)
	{
	case CHG:       Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xFE);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case BUCK1:     Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xFD);
		            Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
		            break;

    case BUCK2:     Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xFB);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case BUCK3:     Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xF7);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case BOOST:     Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xEF);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case BUCK_BOOST:Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xDF);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case LDO1:      Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0xBF);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case LDO2:      Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0x7F);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
	                break;

	case LOCK_UNLOCK_ALL:   Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0x00);
					Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, lock_unlock);
					break;

	default:        Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKMSK, 0x00);
	                Register_Write(I2C_MASTER, MAX20360_SLAVE_ADDR_PMIC, LOCKUNLOCK, UNLOCK);
	                break;
	}

}



float Get_SOC(){

	uint8_t SOC_value[2];
	const float SOC_multiplier = 1.0/256.0;

	Register_Read_FG(I2C_MASTER, MAX20360_SLAVE_ADDR_FUEL_GAUGE, REPSOC, SOC_value);
	float SOC = (0xFFFF & (SOC_value[1]<<8 | SOC_value[0]))*SOC_multiplier;

	return SOC;
}

float Get_Cell_Voltage()
{
	uint8_t Vcell_value[2];
	const float Vcell_multiplier = 78.125e-6;

	Register_Read_FG(I2C_MASTER, MAX20360_SLAVE_ADDR_FUEL_GAUGE, VCELL, Vcell_value);
	float Vcell = (0xFFFF & (Vcell_value[1]<<8 | Vcell_value[0]))*Vcell_multiplier;

	return Vcell;

}


















































