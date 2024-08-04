

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ADBMSCMDLIST.h"
#include "ADBMSCOMMON.h"

int user_command;
/* Set Under Voltage and Over Voltage Thresholds */
const double OV_THRESHOLD = 4.2;                 /* Volt */
const double UV_THRESHOLD = 3.0;                 /* Volt */

//Refon = 1 and All GPIO are OFF
uint8_t CFGA_data[6] = {0x81, 0x00, 0x00, 0xff, 0x03, 0x00};// 0x02, 0x8E
uint8_t CFGB_data[6];
uint8_t PWMA_data[6] = {0x0F, 0x00, 0x00, 0x00, 0x00, 0x00}; //set 100% PWM for cell 1

void adbmsmain(void)
{
	printMenu();
	ADBMS_Init(TOTAL_IC);
	while(1)
	 {
	   scanf("%d", &user_command);
	   printf("Enter command:%d\n", user_command);
	   run_command(user_command);
	   fflush(stdin);
	 }
}

void printMenu(void)
{ 
  printf("\nBMS Initialization\n");
  printf("No of IC = %d\n", TOTAL_IC);
  printf("SPI Initialization\n");
  printf("Four Wire Full Duplex SPI \nCLK Speed = %d\n", SPI_SPEED);
  printf("SPI Mode = %d \nNo of Slaves = %d\nSlave Select Polarity = %d\n\n", SPI_MODE_0, Slave_1, SS_Polarity);
  printf("List of ADBMS6830 commands and their numbers:\n");
  printf("Command No. : Command description\n");
  printf("1           : Write and read configuration\n");
  printf("2           : Measure cell voltages\n");
  printf("3           : Measure s-channel voltages\n");
  printf("4           : Measure average cell voltages\n");
  printf("5           : Measure gpio pin voltages\n");
  printf("6           : Measure status register voltages\n");
  printf("7           : Check PWM Discharge at perticular cell\n");
  printf("\n");
  printf("Enter '0' for menu\n");
  printf("Please enter command: \n");
}


void run_command(int command)
{
 switch(command)
 {
 case 0:
    printMenu();
    break;

 case 1:
    ADBMS_Write_Read_Config(TOTAL_IC);
    break;

 case 2:
    ADC_Cell_Voltage_Measurement(TOTAL_IC);
    break;

  case 3:
    ADC_S_Voltage_Measurement(TOTAL_IC);
    break;

  case 4:
    ADC_AVG_Cell_Voltage_Measurement(TOTAL_IC);
    break;

  case 5:
	  ADC_GPIO_Voltage_Measurement(TOTAL_IC);
    break;

  case 6:
	  ADBMS_Status_Reg_voltage_measurment(TOTAL_IC);
    break;

  case 7:
	  ADSV_PWM_Discharge_Measurement(TOTAL_IC);
      break;

 default:
    printf("Incorrect Option\n\n");
    break;
 }
}

/*!
  @brief Initialized  BMS IC 
*/
void ADBMS_Init(uint8_t tIC)
{
  int Reg_Size =RECEIVE_RD_PACKET_SIZE;
	uint8_t *Data_Read;
	Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); 
	ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
	ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], GRPA); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC(); 
  WRCFGB_data_Set(TOTAL_IC, WRCFGB, UV_THRESHOLD, OV_THRESHOLD, &CFGB_data[0]);
	ADBMS_Write_Data(TOTAL_IC, WRCFGB, &CFGB_data[0]); // Set UV and OV Threshold for all Cells
  ADBMS_Read_Data(TOTAL_IC, RDCFGB, &Data_Read[0], GRPB);
  free(Data_Read);
}

/*!
  @brief Set Refon = 1, UV = 3V and OV = 4.2V in CFG Ref
*/
void ADBMS_Write_Read_Config(uint8_t tIC)
{
  int Reg_Size =RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
  WakeupBMSIC(); 
	ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
	ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], GRPA); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  printf("\nWRCFGA\n");
  ADBMS_Print_WRCFG_Data(TOTAL_IC,  WRCFGA, CFGA_data, GRPA);
  printf("\nRDCFGA\n");
  ADBMS_Print_RDCFG_Data(TOTAL_IC,  RDCFGA, Data_Read, GRPA);
  WRCFGB_data_Set(TOTAL_IC, WRCFGB, UV_THRESHOLD, OV_THRESHOLD, &CFGB_data[0]);
	ADBMS_Write_Data(TOTAL_IC, WRCFGB, &CFGB_data[0]); // Set UV and OV Threshold for all Cells
  ADBMS_Read_Data(TOTAL_IC, RDCFGB, &Data_Read[0], GRPB);
  printf("\nWRCFGB\n");
  ADBMS_Print_WRCFG_Data(TOTAL_IC,  WRCFGB, CFGB_data, GRPB);
  printf("\nRDCFGB\n");
  ADBMS_Print_RDCFG_Data(TOTAL_IC,  RDCFGB, Data_Read, GRPB);
  free(Data_Read);
}

/*!
  @brief Measure C-ADC Voltage in Single shot mode
*/
void ADC_Cell_Voltage_Measurement(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
	ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
	WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
  ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
  ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC();
  ADBMS_ADCV(SINGLESHOT_ADCV, RD_OFF_ADCV, NO_RESET_ADCV, NODISCHARGE_ADCV, ALL_CH_OW_OFF_ADCV, ADCV);
  ADBMS_Write_Cmd(TOTAL_IC, ADCV); //Send ADCV command with Single shot, No Redundancy, no discharge
  ADBMSPollADC(TOTAL_IC, PLCADC);  //Send poll ADC command to Check ADC Conversion
  //MXC_Delay(1100);               //We can use delay as C-ADC conversion completed after 1msec
  WakeupBMSIC();
  ADBMS_Read_Data(TOTAL_IC,   RDCVA, &Data_Read[0], GRPA); // Read Cell Voltages
  ADBMS_Print_Data(TOTAL_IC,  RDCVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDCVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDCVB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDCVC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDCVC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDCVD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDCVD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDCVE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDCVE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDCVF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDCVF, Data_Read, GRPF);
  printf("\nCLR Command\n");
  ADBMS_Write_Cmd(TOTAL_IC, CLRCELL);                       //Clear cell Voltages
  ADBMS_Read_Data(TOTAL_IC,   RDCVA, &Data_Read[0], GRPA);  //read default value after clearcell command
  ADBMS_Print_Data(TOTAL_IC,  RDCVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDCVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDCVB, Data_Read, GRPB);
  free(Data_Read);
}

/*!
  @brief Measure S-ADC Voltage in Single shot mode
*/
void ADC_S_Voltage_Measurement(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
	ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
	WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
  ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
  ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC();
  ADBMS_ADSV(Singleshot_ADSV, NODISCHARGE_ADSV, ALL_CH_OW_OFF_ADSV, ADSV);
  ADBMS_Write_Cmd(TOTAL_IC, ADSV);
  ADBMSPollADC(TOTAL_IC, PLADC);
  //MXC_Delay(15000);
  ADBMS_Read_Data(TOTAL_IC,   RDSVA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDSVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDSVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDSVB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDSVC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDSVC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDSVD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDSVD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDSVE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDSVE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDSVF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDSVF, Data_Read, GRPF);
  printf("\nCLR Command\n");
  ADBMS_Write_Cmd(TOTAL_IC, CLRSPIN);
  ADBMS_Read_Data(TOTAL_IC,   RDSVA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDSVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDSVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDSVB, Data_Read, GRPB);
  free(Data_Read);
}

/*!
  @brief Measure Averge C-ADC Voltage
*/
void ADC_AVG_Cell_Voltage_Measurement(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
	ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
	WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
  ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
  ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC();
  ADBMS_ADCV(CONT_ADCV, RD_OFF_ADCV, NO_RESET_ADCV, NODISCHARGE_ADCV, ALL_CH_OW_OFF_ADCV, ADCV);
  ADBMS_Write_Cmd(TOTAL_IC, ADCV);
  MXC_Delay(8100);
  //pollVoltCTSconversion();
  WakeupBMSIC();
  ADBMS_Read_Data(TOTAL_IC,   RDACA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDACA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDACB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDACB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDACC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDACC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDACD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDACD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDACE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDACE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDACF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDACF, Data_Read, GRPF);
  printf("\nCLR Command\n");
  ADBMS_Write_Cmd(TOTAL_IC, CLRCELL);
  free(Data_Read);
}

/*!
  @brief Measure GPIO Voltage
*/
void ADC_GPIO_Voltage_Measurement(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
	ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
	WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
	ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
  ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC();
  ADBMS_ADAX(OW_AUX_OFF_ADAX, ALL_ADAX, PULLDOWN_ADAX, ADAX);
  ADBMS_Write_Cmd(TOTAL_IC, ADAX);
  ADBMSPollADC(TOTAL_IC, PLAUX1);
  //MXC_Delay(20000); // 20msec delay to get data of all GPIO Regs
  ADBMS_Read_Data(TOTAL_IC,       RDAUXA, &Data_Read[0], GRPA);
  ADBMS_Print_AUX_Data(TOTAL_IC,  RDAUXA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,       RDAUXB, &Data_Read[0], GRPB);
  ADBMS_Print_AUX_Data(TOTAL_IC,  RDAUXB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,       RDAUXC, &Data_Read[0], GRPC);
  ADBMS_Print_AUX_Data(TOTAL_IC,  RDAUXC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,       RDAUXD, &Data_Read[0], GRPD);
  ADBMS_Print_AUX_Data(TOTAL_IC,  RDAUXD, Data_Read, GRPD);
  printf("\nCLR Command\n");
  ADBMS_Write_Cmd(TOTAL_IC, CLRAUX);
  free(Data_Read);
}

/*!
  @brief Measure Status Voltage
*/
void ADBMS_Status_Reg_voltage_measurment(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
	uint8_t *Data_Read;
	Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
	WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
	ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
	WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
	ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
	ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS respond to back to MCU on MISO (SDO) line
  WakeupBMSIC();
	ADBMS_ADAX(OW_AUX_OFF_ADAX, ALL_ADAX, PULLDOWN_ADAX, ADAX);
	ADBMS_Write_Cmd(TOTAL_IC, ADAX);
	ADBMSPollADC(TOTAL_IC, PLAUX1);
  //MXC_Delay(20000); // 20msec delay to get data of all GPIO Regs
  WakeupBMSIC();
  ADBMS_Read_Data(TOTAL_IC,         RDSTATA, &Data_Read[0], GRPA);
  ADBMS_Print_Status_Data(TOTAL_IC, RDSTATA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,         RDSTATB, &Data_Read[0], GRPB);
  ADBMS_Print_Status_Data(TOTAL_IC, RDSTATB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,         RDSTATC, &Data_Read[0], GRPC);
  ADBMS_Read_Data(TOTAL_IC,         RDSTATD, &Data_Read[0], GRPD);
  ADBMS_Read_Data(TOTAL_IC,         RDSTATE, &Data_Read[0], GRPE);
  printf("\nCLR Command\n");
  ADBMS_Write_Cmd(TOTAL_IC, CLRAUX);
  ADBMS_Write_Cmd(TOTAL_IC, CLRFLAG);
  free(Data_Read);
}

/*!
  @brief Check PWM_Discharge at perticular cell
*/
void ADSV_PWM_Discharge_Measurement(uint8_t tIC)
{
  int   Reg_Size=RECEIVE_RD_PACKET_SIZE;
  uint8_t *Data_Read;
  Data_Read = (uint8_t *)calloc(Reg_Size, sizeof(uint8_t));
  WakeupBMSIC(); //Wakeup BMS IC and IsoSPI
  ADBMS_Write_Cmd(TOTAL_IC, &SRST[0]); // Reset ADBMS (ADBMS will goes into sleep state)
  WakeupBMSIC(); ////Wakeup BMS IC and IsoSPI after Reset to bring ADBMS from SLEEP to STANDBY
  ADBMS_Write_Data(TOTAL_IC, WRCFGA, &CFGA_data[0]); // Set REFON = 1 to bring ADBMS from Standby to REFUP State
  ADBMS_Read_Data(TOTAL_IC, RDCFGA, &Data_Read[0], NOT_ALL); //RDCFGA data should be match with WRCFGA data. RDCFGA helps to make sure that ADBMS responds back to MCU on MISO (SDO) line

  //Send ADSV with DCP = 0 and DCTO = 0 and check discharge should not happen
  printf("\nSend ADSV with DCP = 0 and DCTO = 0 and check discharge should not happen\n");
  WakeupBMSIC();
  ADBMS_ADSV(Singleshot_ADSV, NODISCHARGE_ADSV, ALL_CH_OW_OFF_ADSV, ADSV);
  ADBMS_Write_Cmd(TOTAL_IC, ADSV);
  ADBMSPollADC(TOTAL_IC, PLADC);
  //MXC_Delay(15000);
  ADBMS_Read_Data(TOTAL_IC,   RDSVA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDSVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDSVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDSVB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDSVC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDSVC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDSVD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDSVD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDSVE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDSVE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDSVF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDSVF, Data_Read, GRPF);
  ADBMS_Write_Cmd(TOTAL_IC, CLRSPIN);

  //DCTO = 3min (1 min per bit) in WRCFGB and Send ADSV with DCP = 0 and \ncheck discharge should not happen
  printf("\nSet DCTO = 3min (1 min per bit) in WRCFGB and Send ADSV with DCP = 0 and \ncheck discharge should not happen\n");
  WakeupBMSIC();
  WRCFGB_data_Set(TOTAL_IC, WRCFGB, UV_THRESHOLD, OV_THRESHOLD, &CFGB_data[0]);
  ADBMS_Write_Data(TOTAL_IC, WRCFGB, &CFGB_data[0]); // Set UV and OV Threshold for all Cells and set DCTO for 3min
  ADBMS_Read_Data(TOTAL_IC, RDCFGB, &Data_Read[0], GRPB);

  ADBMS_ADSV(Singleshot_ADSV, NODISCHARGE_ADSV, ALL_CH_OW_OFF_ADSV, ADSV);
  ADBMS_Write_Cmd(TOTAL_IC, ADSV);
  ADBMSPollADC(TOTAL_IC, PLADC);
  //MXC_Delay(15000);
  ADBMS_Read_Data(TOTAL_IC,   RDSVA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDSVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDSVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDSVB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDSVC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDSVC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDSVD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDSVD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDSVE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDSVE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDSVF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDSVF, Data_Read, GRPF);
  ADBMS_Write_Cmd(TOTAL_IC, CLRSPIN);

  //DCTO = 3min (1 min per bit. so, set 00011 for 3min) in WRCFGB and Send ADSV with DCP = 1 and check discharge should happen now
  printf("\nSet DCTO = 3min (1 min per bit. so, set 00011 for 3min) in WRCFGB\nSend WRPWM1 command with 100 percent duty cycle at cell 1 and \nSend ADSV with DCP = 1 and check discharge should happen now\n");
  //WRPWM to discharge the Cell
  WakeupBMSIC();
  WRCFGB_data_Set(TOTAL_IC, WRCFGB, UV_THRESHOLD, OV_THRESHOLD, &CFGB_data[0]);
  ADBMS_Write_Data(TOTAL_IC, WRCFGB, &CFGB_data[0]); // Set UV and OV Threshold for all Cells and set DCTO for 3min
  ADBMS_Read_Data(TOTAL_IC, RDCFGB, &Data_Read[0], GRPB);

  ADBMS_Write_Data(TOTAL_IC, WRPWM1, &PWMA_data[0]); // Set 100% PWM for cell 1
  ADBMS_Read_Data(TOTAL_IC, RDPWM1, &Data_Read[0], GRPA);

  ADBMS_ADSV(Singleshot_ADSV, DISCHARGE_ADSV, ALL_CH_OW_OFF_ADSV, ADSV);
  ADBMS_Write_Cmd(TOTAL_IC, ADSV);
  ADBMSPollADC(TOTAL_IC, PLADC);
  //MXC_Delay(15000);
  ADBMS_Read_Data(TOTAL_IC,   RDSVA, &Data_Read[0], GRPA);
  ADBMS_Print_Data(TOTAL_IC,  RDSVA, Data_Read, GRPA);
  ADBMS_Read_Data(TOTAL_IC,   RDSVB, &Data_Read[0], GRPB);
  ADBMS_Print_Data(TOTAL_IC,  RDSVB, Data_Read, GRPB);
  ADBMS_Read_Data(TOTAL_IC,   RDSVC, &Data_Read[0], GRPC);
  ADBMS_Print_Data(TOTAL_IC,  RDSVC, Data_Read, GRPC);
  ADBMS_Read_Data(TOTAL_IC,   RDSVD, &Data_Read[0], GRPD);
  ADBMS_Print_Data(TOTAL_IC,  RDSVD, Data_Read, GRPD);
  ADBMS_Read_Data(TOTAL_IC,   RDSVE, &Data_Read[0], GRPE);
  ADBMS_Print_Data(TOTAL_IC,  RDSVE, Data_Read, GRPE);
  ADBMS_Read_Data(TOTAL_IC,   RDSVF, &Data_Read[0], GRPF);
  ADBMS_Print_Data(TOTAL_IC,  RDSVF, Data_Read, GRPF);
  ADBMS_Write_Cmd(TOTAL_IC, CLRSPIN);

  free(Data_Read);
}
/**************************Print Function********************************/

/*!
  @brief Print Write CNFG Reg Data
*/
void ADBMS_Print_WRCFG_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type)
{ 
  int Data_Size = 6;
  for(int IC=1; IC<=tIC; IC++)
  {
    for(int i=0; i<(Data_Size); i++)
    {
      printf("0x%x\t",buff[i]);
    }
    printf("\n");
  }
}

/*!
  @brief Print Read CNFG Reg Data
*/
void ADBMS_Print_RDCFG_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type)
{ 
  int Data_Size = 6;
  int buffdata = 0;
  for(int IC=1; IC<=tIC; IC++)
  {
    for(int i=0; i<(Data_Size); i++)
    {
      printf("0x%x\t",buff[buffdata]);
      buffdata++;
    }
    printf("\n");
  }
}

/*!
  @brief Print Cell , S, Avg , Fillterced cell voltages
*/
void ADBMS_Print_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type)
{
  int16_t x, y, z; //to hold raw data
  uint8_t i = 0;
  for(uint8_t IC=0;IC<tIC;IC++)
  {
    x = twos_complement_to_int((buff[i+1]<< 8)|buff[i],16);//Combine lower and upper bytes
    y = twos_complement_to_int((buff[i+3]<< 8)|buff[i+2],16);
    z = twos_complement_to_int((buff[i+5]<< 8)|buff[i+4],16);
    if			  ( type == GRPA){printf("\nIC:0%x->\t CELL1  =%1.3lf\t CELL2 =%1.3lf\t CELL3 =%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(y), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(z));}
    else if		( type == GRPB){printf("\nIC:0%x->\t CELL4  =%1.3lf\t CELL5 =%1.3lf\t CELL6 =%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(y), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(z));}
    else if 	( type == GRPC){printf("\nIC:0%x->\t CELL7  =%1.3lf\t CELL8 =%1.3lf\t CELL9 =%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(y), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(z));}
    else if 	( type == GRPD){printf("\nIC:0%x->\t CELL10 =%1.3lf\t CELL11=%1.3lf\t CELL12=%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(y), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(z));}
    else if 	( type == GRPE){printf("\nIC:0%x->\t CELL13 =%1.3lf\t CELL14=%1.3lf\t CELL15=%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(y), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(z));}
    else if 	( type == GRPF){printf("\nIC:0%x->\t CELL16 =%1.3lf\n",(IC+1), ADBMS_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(x));}
    else	    {printf("error");}
    i +=6;
  }
}

/*!
  @brief Print GPIO voltages
*/
void ADBMS_Print_AUX_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type)
{
  int16_t x, y, z; //to hold raw data
  uint8_t i = 0;
  for(uint8_t IC=0;IC<tIC;IC++)
  {
    x = twos_complement_to_int((buff[i+1]<< 8)|buff[i],16);//Combine lower and upper bytes
    y = twos_complement_to_int((buff[i+3]<< 8)|buff[i+2],16);
    z = twos_complement_to_int((buff[i+5]<< 8)|buff[i+4],16);
    if			  ( type == GRPA){printf("\nIC:0%x->\t  GPIO1  =%1.3lf\t GPIO2 =%1.3lf\t GPIO3 =%1.3lf\n",              (IC+1), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(x), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(y),  ADBMS_CONVERT_GPIO_HEX_TO_VOLT(z));}
    else if		( type == GRPB){printf("\nIC:0%x->\t  GPIO4  =%1.3lf\t GPIO5 =%1.3lf\t GPIO6 =%1.3lf\n",              (IC+1), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(x), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(y),  ADBMS_CONVERT_GPIO_HEX_TO_VOLT(z));}
    else if 	( type == GRPC){printf("\nIC:0%x->\t  GPIO7  =%1.3lf\t GPIO8 =%1.3lf\t GPIO9 =%1.3lf\n",              (IC+1), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(x), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(y),  ADBMS_CONVERT_GPIO_HEX_TO_VOLT(z));}
    else if 	( type == GRPD){printf("\nIC:0%x->\t  GPIO10 =%1.3lf\t VM  = %1.3lf\t  Total Cell Voltage = %1.3lf\n",(IC+1), ADBMS_CONVERT_GPIO_HEX_TO_VOLT(x), ADBMS_CONVERT_VM_HEX_TO_VOLT(y),    ADBMS_CONVERT_VP_HEX_TO_VOLT(z));}
    else	    {printf("error");}
    i +=6;
  }
}

/*!
  @brief Print Status Reg data
*/
void ADBMS_Print_Status_Data(uint8_t tIC, uint8_t cmd_arg[], uint8_t *buff, RD_DATA_SIZE_ALL_TYPE type)
{
  int16_t x, y, z; //to hold raw data
  uint8_t i = 0;
  for(uint8_t IC=0;IC<tIC;IC++)
  {
    x = twos_complement_to_int((buff[i+1]<< 8)|buff[i],16);//Combine lower and upper bytes
    y = twos_complement_to_int((buff[i+3]<< 8)|buff[i+2],16);
    z = twos_complement_to_int((buff[i+5]<< 8)|buff[i+4],16);
    if			  ( type == GRPA){printf("\nIC:0%x->\t  VREF2     = %1.3lf\t IC Dia Temperature = %1.3lf\n", (IC+1), ADBMS_CONVERT_VREF2_HEX_TO_VOLT(x), ADBMS_CONVERT_ITEMP_HEX_TO_VOLT(y));}
    else if		( type == GRPB){printf("\nIC:0%x->\t  VDigital  = %1.3lf\t VAnalog  = %1.3lf\t VRES = %1.3lf\n", (IC+1), ADBMS_CONVERT_VD_HEX_TO_VOLT(x), ADBMS_CONVERT_VA_HEX_TO_VOLT(y), ADBMS_CONVERT_VRES_HEX_TO_VOLT(z));}
    else	    {printf("error");}
    i +=6;
  }
}

/*!
  @brief Check Singleshot ADC conversion
*/
//To check ADC(Single shot) conversion
//Single shot mode only
//Not applicable to Continuous mode
void ADBMSPollADC(uint8_t tIC, uint8_t cmd_arg[2])
{
  int Pec_Size = 2, Cmd_Size =2, Check_ADC_Conversion = 1;
  int WRCmdSize = Cmd_Size + Pec_Size + Check_ADC_Conversion;
  uint16_t cmd_pec;
  uint8_t tx_data[WRCmdSize];//2 byte cmd + 2byte cmd pec
  uint8_t rx_data[WRCmdSize];//2 byte cmd + 2byte cmd pec
  tx_data[0] = cmd_arg[0];
  tx_data[1] = cmd_arg[1];
  cmd_pec = Pec15_Calc(Pec_Size, cmd_arg); //2byte cmd pec
  tx_data[2] = (uint8_t)(cmd_pec >> 8);
  tx_data[3] = (uint8_t)(cmd_pec);
  WakeupBMSIC();
  do
  {
  SPI_Transaction(tx_data, rx_data, WRCmdSize);
  }while(rx_data[4] <0x03); //It make sure that SDO will go high after ADC conversion will done.
  #if Test_Print
      printf("\nCommand\n");
      printf("0x%x\t0x%x\t0x%x\t0x%x\t0x%x\n",tx_data[0], tx_data[1],tx_data[2], tx_data[3], tx_data[4]);
  #endif
}

/*!
  @brief This function polls the STATC register until CTS_V bit is reset back to 0
*/
//To check C-ADC(Contimuous) conversion
//Contimuous mode only
//Not applicable to singleshot mode
void pollVoltCTSconversion(void)
{
  uint8_t Data_Read[8];
  do
  {
    /*Read Status Register*/
    ADBMS_Read_Data(TOTAL_IC, RDSTATC, &Data_Read[0], GRPC);
    #if 1
        printf("\nRDSTATC=%x\n", (Data_Read[1] & 3U));
    #endif
  } while (((Data_Read[3] & 3U) != 3U));

  do
  {
    /*Read Status Register*/
    ADBMS_Read_Data(TOTAL_IC, RDSTATC, &Data_Read[0], GRPC);
    #if 1
        printf("RDSTATC=%x\n", (Data_Read[1] & 3U));
    #endif
  } while (Data_Read[3] & 3U); // It will come out when CTS_V become 0
}

/*!
  @brief This function polls the STATG register until CTS_V bit is reset back to 0, 8 times
*/
//Only for C ADC and S ADC
//Contimuous mode only
//Not applicable to singleshot mode
void poll8msCtsVconversion(void)
{
  uint8_t vpoll_count = 8;
  for (uint8_t i = 0; i < vpoll_count; i++)
  {
    pollVoltCTSconversion();
  }
}




