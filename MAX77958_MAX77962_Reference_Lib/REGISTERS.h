/*
 * MAX77958_Registers.h
 *
 *  Created on: Sep 28, 2023
 *      Author: Fahad Ahammad
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

///I2C Defines///

#define I2C_MASTER MXC_I2C0  //P0.8 & P0.9

///GPIO Pins///

#define BUTTON_PORT MXC_GPIO0
#define BUTTON_PIN MXC_GPIO_PIN_3

#define LED_PORT MXC_GPIO0
#define LED_PIN MXC_GPIO_PIN_13

///MAX77958 BASIC REGISTERS///

#define  DEVICE_ID     0x00
#define  DEVICE_REV    0x01
#define  FW_REV        0x02
#define  FW_SUB_VER    0x03
#define  UIC_INT       0x04
#define  CC_INT        0x05
#define  PD_INT        0x06
#define  ACTION_INT    0x07
#define  USBC_STATUS1  0x08
#define  USBC_STATUS2  0x09
#define  BC_STATUS     0x0A
#define  DP_STATUS     0x0B
#define  CC_STATUS0    0x0C
#define  CC_STATUS1    0x0D
#define  PD_STATUS0    0x0E
#define  PD_STATUS1    0x0F
#define  UIC_INT_M     0x10
#define  CC_INT_M      0x11
#define  PD_INT_M      0x12
#define  ACTION_INT_M  0x13

///MAX77958 COMMAND REGISTERS///

#define BC_CTRL1_CONFIG_READ     0x01
#define BC_CTRL1_CONFIG_WRITE    0x02
#define BC_CTRL2_CONFIG_READ     0x03
#define BC_CTRL2_CONFIG_WRITE    0x04
#define CONTROL1_READ            0x05
#define CONTROL_WRITE            0x06
#define CC_CONTROL1_READ         0x0B
#define CC_CONTROL1_WRITE        0x0C
#define CC_CONTROL4_READ         0x11
#define CC_CONTROL4_WRITE        0x12
#define GPIO_CONTROL_READ        0x23
#define GPIO_CONTROL_WRITE       0x24
#define GPIO0_GPIO1_ADC_READ     0x27
#define GET_SNK_CAP              0x2F
#define CUR_SEL_SRC_CAP          0x30
#define GET_SRC_CAP              0x31
#define SRC_CAP_REQ              0x32
#define SET_SRC_CAP              0x33
#define SEND_GET_REQ             0x34
#define READ_GET_REQ_RESP        0x35
#define SEND_GET_RESP            0x36
#define SWAP_REQ                 0x37
#define SWAP_REQ_RESPONSE        0x38
#define APDO_SRCCAP_REQUEST      0x3A
#define SET_PPS                  0x3C
#define SNK_PDO_REQUEST_READ     0x3E
#define SNK_PDO_SET              0x3F
#define GETPDMSG                 0x4A
#define CUSTOM_CONFIG_READ       0x55
#define CUSTOM_CONFIG_WRITE      0x56
#define GPIO7_GPIO8_INT_SET_REQ  0x64
#define MASTER_I2C_READ          0x85
#define MASTER_I2C_WRITE         0x86


///////MAX77962 Registers//////

#define SLAVE_WRITE_ADDRESS 0xD2
#define SLAVE_READ_ADDRESS  0xD3

#define CHG_CNFG_00  0x16
#define CHG_CNFG_02  0x18
#define CHG_CNFG_04  0x1A
#define CHG_CNFG_08  0x1E





#endif /* REGISTERS_H_ */
