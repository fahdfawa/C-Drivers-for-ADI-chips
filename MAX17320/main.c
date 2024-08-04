///Developed by Fahad///
///CAC APR////

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2c.h"
#include "dma.h"


/***** Definitions *****/
void Determine_Remaining_Updates();
void init();
int Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value);
int Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB, uint8_t Write_value0_MSB);
int Register_Write_NV(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB, uint8_t Write_value0_MSB);
int Register_Read_NV(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value);

// #define MASTERDMA

#define I2C_MASTER MXC_I2C0  //P0.6 and P0.7
//#define I2C_SLAVE MXC_I2C1

#define I2C_FREQ 100000
// This example may become unreliable at I2C frequencies above 100kHz.
// This is only an issue in the loopback configuration, where the I2C block is
// connected to itself.
#define I2C_SLAVE_ADDR (0x36)
#define I2C_BYTES 100

/***** Globals *****/
//static uint8_t Stxdata[I2C_BYTES];
//static uint8_t Srxdata[I2C_BYTES];
static uint8_t txdata[I2C_BYTES];
static uint8_t rxdata[I2C_BYTES];
volatile uint8_t DMA_FLAG = 0;
volatile int I2C_FLAG;
volatile int txnum = 0;
volatile int txcnt = 0;
volatile int rxnum = 0;
volatile int num;

float voltage_scale = 78.125e-6;
/***** Functions *****/
mxc_i2c_req_t reqMaster;

//Slave interrupt handler


//I2C callback function
void I2C_Callback(mxc_i2c_req_t *req, int error)
{
    I2C_FLAG = error;
    return;
}


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

//Compare data to see if they are the same
int verifyData()
{
    int i, fails = 0;

    for (i = 0; i < I2C_BYTES; ++i) {
        if (txdata[i] != rxdata[i]) {
            ++fails;
        }
    }

    if (fails > 0) {
        return E_FAIL;
    }

    return E_NO_ERROR;
}

// *****************************************************************************
int main()
{



    int error, i = 0;

    //Setup the I2CM
    error = MXC_I2C_Init(I2C_MASTER, 1, 0);

    if (error != E_NO_ERROR) {
        printf("-->Failed master\n");
        return error;
    } else {
        printf("\n-->I2C Master Initialization Complete");
    }

    NVIC_EnableIRQ(I2C0_IRQn);

    MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ);


    uint8_t commstat[2];
    uint8_t config[2];

    Determine_Remaining_Updates();
    Register_Write(I2C_MASTER, 0x13, 0x55, 0x00);


     ///Un-comment this to write to NV memory


//    do{
//
//    Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);//Commstat register
//    Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);
//
//    MXC_Delay(10000);
//
//    Register_Write(I2C_MASTER, 0x9C, 0x40, 0x06);//command register
//    //Register_Write_NV(I2C_MASTER, 0xB5, 0x02, 0x80);
//    init();
//    Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);
//    Register_Write(I2C_MASTER, 0x60, 0x04, 0xE9);
//
//    MXC_Delay(400000);
//
//    Register_Read(I2C_MASTER, 0x61, commstat);
//
//    }while((commstat[1] & 0x00000100) == 0x00000100);
//
//    Register_Write(I2C_MASTER, 0x60, 0x0F, 0x00);
//    MXC_Delay(10000);
//
//    Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);
//    MXC_Delay(10);
//    Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);
//
//    Register_Write(I2C_MASTER, 0xAB, 0x00, 0x80);
//
//    Register_Read(I2C_MASTER, 0xAB, config);
//
//    while((config[2] & 0x10000000) == 0x10000000)
//    {
//    }

    // This will write data to slave
  // Then read data back from slave
    while(1)
    {
    uint16_t DevName, Vcell, Vcell1, Vcell2, Vcell3, Vcell4, RepSOC, Packreg, Protalrt, FullSOCthreshold, Protection, VFSOC;
    uint16_t Design_cap;
    uint8_t data[2], data0[2], data1[2], data2[2], data3[2], data4[2], soc[2], pack[2], prot[2], fullsocthr[2], protstatus[2], vfs[2];
    uint8_t dat[2];
    uint8_t Pack_conf[2];

//    Register_Read_NV(I2C_MASTER, 0xB5, Pack_conf);
//	uint16_t Pkcf = 0xFFFF & ((Pack_conf[1] << 8) | Pack_conf[0]);
//	printf("Pack Conf = %x\n", Pkcf);

    //Reading Vcell

    Register_Read(I2C_MASTER, 0x21, data);
    DevName = 0xFFFF & ((data[1] << 8) | data[0]);
    printf("Dev Name = %x\n", DevName);

    Register_Read(I2C_MASTER, 0x13, fullsocthr);
    FullSOCthreshold = 0xFFFF & ((fullsocthr[1] << 8) | fullsocthr[0]);
    printf("FullSOCthr = %x\n", FullSOCthreshold);

    Register_Read(I2C_MASTER, 0xFF, vfs);
    VFSOC = 0xFFFF & ((vfs[1] << 8) | vfs[0]);
    printf("VFSOC = %x\n", VFSOC);

    Register_Read(I2C_MASTER, 0xD9, protstatus);
    Protection = 0xFFFF & ((protstatus[1] << 8) | protstatus[0]);
    printf("ProtectionStatus = %x\n", Protection);

    Register_Read(I2C_MASTER, 0x1A, data0);
    Vcell = 0xFFFF & ((data0[1] << 8) | data0[0]);
    float Vcell_in_volt = Vcell * voltage_scale;
    //printf("V_cell = %X\n",Vcell);
    printf("V_cell_in_volt = %f\n", Vcell_in_volt);

    Register_Read(I2C_MASTER, 0xD1, data1);
    Vcell1 = 0xFFFF & ((data1[1] << 8) | data1[0]);
    float Vcell1_in_volt = Vcell1 * voltage_scale;
    printf("V_cell1_in_volt = %f\n", Vcell1_in_volt);
    //printf("V_cell1_in_HEX = %X\n", Vcell1);

    Register_Read(I2C_MASTER, 0xD2, data2);
    Vcell2 = 0xFFFF & ((data2[1] << 8) | data2[0]);
    float Vcell2_in_volt = Vcell2 * voltage_scale;
    printf("V_cell2_in_volt = %f\n", Vcell2_in_volt);

    Register_Read(I2C_MASTER, 0xD3, data3);
    Vcell3 = 0xFFFF & ((data3[1] << 8) | data3[0]);
    float Vcell3_in_volt = Vcell3 * voltage_scale;
    printf("V_cell3_in_volt = %f\n", Vcell3_in_volt);

    Register_Read(I2C_MASTER, 0xD4, data4);
    Vcell4 = 0xFFFF & ((data4[1] << 8) | data4[0]);
    float Vcell4_in_volt = Vcell4 * voltage_scale;
    printf("V_cell4_in_volt = %f\n", Vcell4_in_volt);

//    Register_Read(I2C_MASTER, 0xDB, pack);
//    Packreg = 0xFFFF & ((pack[1] << 8) | pack[0]);
//    float Pack_in_volt = Packreg * 0.0003125;
//    printf("V_Pack_in_volt = %f\n", Pack_in_volt);
//    printf("V_Pack_in_hex = %x\n", Packreg);
//
//    Register_Read(I2C_MASTER, 0x06, soc);
//    RepSOC = 0xFFFF & ((soc[1] << 8) | soc[0]);
//    float RepSOC_in_percentage = (float)(RepSOC / 256);
//    printf("RepSOC = %x\n", RepSOC);
//
//    Register_Read(I2C_MASTER, 0xD7, prot);
//    Protalrt = 0xFFFF & ((prot[1] << 8) | prot[0]);
//    printf("ProtAlrt = %x\n", Protalrt);
//    writing and Reading Design capacity

//    Register_Write(I2C_MASTER, 0x18, 0x98, 0x3A);
//    Register_Read(I2C_MASTER, 0x18, dat);
//    Design_cap = 0xFFFF & ((dat[1] << 8) | dat[0]);
//    printf("Design_cap = %X\n", Design_cap);
    MXC_Delay(1000000);
 }
//
}

int Register_Read(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value)
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x36;
	uint8_t buf[1] = {reg};
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_buf = Read_value;                               //Re
	reqMaster.rx_len = 2;                                        //Please change this value for multiple read data
	return MXC_I2C_MasterTransaction(&reqMaster);
}

int Register_Write(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB, uint8_t Write_value0_MSB) //
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x36;
	uint8_t buf[3] = {reg, Write_value0_LSB, Write_value0_MSB }; // is write data is more than 16bit, add Write_value1_LSB, Write_value1_MSB and so on...
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_len = 0;
	return MXC_I2C_MasterTransaction(&reqMaster);
}

int Register_Write_NV(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t Write_value0_LSB, uint8_t Write_value0_MSB) //
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x0B;
	uint8_t buf[3] = {reg, Write_value0_LSB, Write_value0_MSB }; // is write data is more than 16bit, add Write_value1_LSB, Write_value1_MSB and so on...
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_len = 0;
	return MXC_I2C_MasterTransaction(&reqMaster);
}


int Register_Read_NV(mxc_i2c_regs_t* i2c_master, uint8_t reg, uint8_t* Read_value)
{
	reqMaster.i2c = i2c_master;
	reqMaster.addr = 0x0B;
	uint8_t buf[1] = {reg};
	reqMaster.tx_buf = buf;
	reqMaster.tx_len = sizeof(buf);
	reqMaster.rx_buf = Read_value;                               //Re
	reqMaster.rx_len = 2;                                        //Please change this value for multiple read data
	return MXC_I2C_MasterTransaction(&reqMaster);
}

void Determine_Remaining_Updates()
{
 uint8_t Ret_Value[2];
 Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);
 Register_Write(I2C_MASTER, 0x61, 0x00, 0x00);

 Register_Write(I2C_MASTER, 0x60, 0x9B, 0xE2);

 MXC_Delay(10000);

 Register_Read_NV(I2C_MASTER, 0xFD, Ret_Value);

 printf("Upper Byte: %x\n", Ret_Value[1]);
 printf("Lower Byte: %x\n", Ret_Value[0]);

}

void init()
{
	Register_Write_NV(I2C_MASTER, 0x80, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x81, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x82, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x83, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x84, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x85, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x86, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x87, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x88, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x89, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8A, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8B, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8C, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8D, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8E, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x8F, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x90, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x91, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x92, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x93, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x94, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x95, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x96, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x97, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x98, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x99, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x9A, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x9B, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x9C, 0x40, 0x06);
	Register_Write_NV(I2C_MASTER, 0x9D, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x9E, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0x9F, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xA0, 0x50, 0x10);
	Register_Write_NV(I2C_MASTER, 0xA1, 0x02, 0x80);
	Register_Write_NV(I2C_MASTER, 0xA2, 0x8C, 0x07);
	Register_Write_NV(I2C_MASTER, 0xA3, 0x80, 0x08);
	Register_Write_NV(I2C_MASTER, 0xA4, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xA5, 0xB0, 0x0C);
	Register_Write_NV(I2C_MASTER, 0xA6, 0xCC, 0x08);
	Register_Write_NV(I2C_MASTER, 0xA7, 0x3E, 0x22);
	Register_Write_NV(I2C_MASTER, 0xA8, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xA9, 0xF0, 0x0A);
	Register_Write_NV(I2C_MASTER, 0xAA, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xAB, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xAC, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xAD, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xAE, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xAF, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xB0, 0x90, 0x02);
	Register_Write_NV(I2C_MASTER, 0xB1, 0x04, 0x02);
	Register_Write_NV(I2C_MASTER, 0xB2, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xB3, 0xF0, 0x0A);
	Register_Write_NV(I2C_MASTER, 0xB4, 0x08, 0x00);
	Register_Write_NV(I2C_MASTER, 0xB5, 0x06, 0x00);
	Register_Write_NV(I2C_MASTER, 0xB6, 0x3B, 0x08);
	Register_Write_NV(I2C_MASTER, 0xB7, 0x41, 0x22);
	Register_Write_NV(I2C_MASTER, 0xB8, 0x80, 0x0A);
	Register_Write_NV(I2C_MASTER, 0xB9, 0x82, 0x01);
	Register_Write_NV(I2C_MASTER, 0xBA, 0x2D, 0xBE);
	Register_Write_NV(I2C_MASTER, 0xBB, 0x09, 0x09);
	Register_Write_NV(I2C_MASTER, 0xBC, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xBD, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xBE, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xBF, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC0, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC1, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC2, 0x61, 0x20);
	Register_Write_NV(I2C_MASTER, 0xC3, 0xE1, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC4, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC5, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC6, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC7, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xC8, 0x00, 0x40);
	Register_Write_NV(I2C_MASTER, 0xC9, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xCA, 0xBE, 0x71);
	Register_Write_NV(I2C_MASTER, 0xCB, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xCC, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xCD, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xCE, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xCF, 0x64, 0x00);
	Register_Write_NV(I2C_MASTER, 0xD0, 0x8C, 0x50);
	Register_Write_NV(I2C_MASTER, 0xD1, 0x00, 0x37);
	Register_Write_NV(I2C_MASTER, 0xD2, 0x28, 0x55);
	Register_Write_NV(I2C_MASTER, 0xD3, 0xB5, 0x4B);
	Register_Write_NV(I2C_MASTER, 0xD4, 0x60, 0x04);
	Register_Write_NV(I2C_MASTER, 0xD5, 0x0A, 0x2D);
	Register_Write_NV(I2C_MASTER, 0xD6, 0x58, 0x7A);
	Register_Write_NV(I2C_MASTER, 0xD7, 0x00, 0x09);
	Register_Write_NV(I2C_MASTER, 0xD8, 0x4B, 0x64);
	Register_Write_NV(I2C_MASTER, 0xD9, 0x59, 0x00);
	Register_Write_NV(I2C_MASTER, 0xDA, 0x54, 0xB7);
	Register_Write_NV(I2C_MASTER, 0xDB, 0x84, 0xC8);
	Register_Write_NV(I2C_MASTER, 0xDC, 0x3D, 0xAB);
	Register_Write_NV(I2C_MASTER, 0xDD, 0xAF, 0x0E);
	Register_Write_NV(I2C_MASTER, 0xDE, 0x55, 0x43);
	Register_Write_NV(I2C_MASTER, 0xDF, 0xA3, 0xA0);
	Register_Write_NV(I2C_MASTER, 0xE0, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE1, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE2, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE3, 0xB9, 0xA5);
	Register_Write_NV(I2C_MASTER, 0xE4, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE5, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE6, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE7, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE8, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xE9, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xEA, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xEB, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xEC, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xED, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xEE, 0x00, 0x00);
	Register_Write_NV(I2C_MASTER, 0xEF, 0x00, 0x00);


}




