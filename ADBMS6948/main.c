
/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mxc_device.h"
#include "mxc_pins.h"
#include "spi.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "nvic_table.h"
#include "uart.h"
#include "board.h"
#include "ADBMSCOMMON.h"
#include "max32690.h"

///***** Initialization of MCU *****/
#define 	SPI               	MXC_SPI0

/* SPI0_SS */
#define MAX_GPIO_P2_26_SPI0_SS1_PORT_IN			MXC_GPIO2
#define MAX_GPIO_P2_26_SPI0_SS1_PIN_IN			MXC_GPIO_PIN_26

mxc_gpio_cfg_t l_s_spi_ss1_config;

void adi_max32690_GPIO_Init(void);
mxc_gpio_cfg_t 			  l_s_r_LED_config;
mxc_gpio_cfg_t 			  l_s_g_LED_config;
mxc_gpio_cfg_t 			  l_s_fet_en_config;
mxc_gpio_cfg_t 			  l_s_usr_btn_config;
mxc_gpio_cfg_t 			  l_s_spi_ss1_config;
mxc_gpio_cfg_t 			  l_s_pchg_fet_config;

/*
typedef enum {
    SPI_MODE_0, ///< clock phase = 0, clock polarity = 0
    SPI_MODE_1, ///< clock phase = 0, clock polarity = 1
    SPI_MODE_2, ///< clock phase = 1, clock polarity = 0
    SPI_MODE_3, ///< clock phase = 1, clock polarity = 1
} mxc_spi_mode_t;
*/
int main(void)
{
	/* GPIO Initialize */
	adi_max32690_GPIO_Init();
	MXC_GPIO_OutSet(MXC_GPIO1, MXC_GPIO_PIN_8);
	/* Start - Microcontroller Initialization */
	mxc_spi_pins_t spi_pins;
    spi_pins.ss0 = FALSE;
    spi_pins.ss1 = TRUE;
    spi_pins.ss2 = FALSE;
	MXC_SPI_Init(SPI, Master_Mode, Mono_Mode, Slave_1, SS_Polarity, SPI_SPEED, spi_pins);
	MXC_SPI_SetDataSize(SPI, 8);				//Set Datasize of rxdata and txdata
	MXC_SPI_SetMode(SPI, SPI_MODE_0);			//Mode 0 SPI for BMS IC
	MXC_SPI_SetWidth(SPI, SPI_WIDTH_STANDARD); 	// ///< MISO/MOSI, full duplex
	/* User Code ---- starts here -------- */
	//MXC_GPIO_OutClr(MXC_GPIO1, MXC_GPIO_PIN_8);
	//MXC_Delay(1000);

	/* ADBMS6948 */
	adbmsmain();

    return 0;
}

void SPI_Transaction(uint8_t *tx_dat, uint8_t *rx_dat, uint8_t length)
{
  mxc_spi_req_t req;
  req.spi = SPI;
  req.txData = tx_dat; 		//Tx_Buffer
  req.rxData = rx_dat; 		//Rx_Buffer
  req.txLen = length;
  req.rxLen = length;
  req.ssIdx = 1; 			// Chip select index = 1 (if spi_pins.ss1 is TRUE;)
  req.txCnt = 0; 			// Initialise transmit counter to track no. of bytes transmitted
  req.rxCnt = 0; 			// Initialise receive counter to track no. of bytes received
  req.ssDeassert = 1; 		// Chip select to high after transaction is complete
  MXC_SPI_MasterTransaction(&req);
}

void adi_max32690_GPIO_Init(void)
{
	/* User R_LED */
//	mxc_gpio_cfg_t 			  l_s_r_LED_config;
	l_s_r_LED_config.port	= MXC_GPIO0;
	l_s_r_LED_config.mask	= MXC_GPIO_PIN_14;
	l_s_r_LED_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_r_LED_config.func	= MXC_GPIO_FUNC_OUT;
	l_s_r_LED_config.vssel	= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_r_LED_config);

	/* User G_LED */
//	mxc_gpio_cfg_t 			  l_s_g_LED_config;
	l_s_g_LED_config.port	= MXC_GPIO0;
	l_s_g_LED_config.mask	= MXC_GPIO_PIN_23;
	l_s_g_LED_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_g_LED_config.func	= MXC_GPIO_FUNC_OUT;
	l_s_g_LED_config.vssel	= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_g_LED_config);


	/* User MCU_FET_EN */
//	mxc_gpio_cfg_t 			  l_s_fet_en_config;
	l_s_fet_en_config.port	= MXC_GPIO1;
	l_s_fet_en_config.mask	= MXC_GPIO_PIN_8;
	l_s_fet_en_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_fet_en_config.func	= MXC_GPIO_FUNC_OUT;
	l_s_fet_en_config.vssel	= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_fet_en_config);

	/* User PCHG_FET_EN */
//	mxc_gpio_cfg_t 			  l_s_fet_en_config;
	l_s_pchg_fet_config.port= MXC_GPIO1;
	l_s_pchg_fet_config.mask= MXC_GPIO_PIN_19;
	l_s_pchg_fet_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_pchg_fet_config.func= MXC_GPIO_FUNC_OUT;
	l_s_pchg_fet_config.vssel = MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_pchg_fet_config);

	/* User_BTN */
//	mxc_gpio_cfg_t 			  l_s_usr_btn_config;
	l_s_usr_btn_config.port	= MXC_GPIO4;
	l_s_usr_btn_config.mask	= MXC_GPIO_PIN_0;
	l_s_usr_btn_config.pad	= MXC_GPIO_PAD_PULL_UP;
	l_s_usr_btn_config.func	= MXC_GPIO_FUNC_IN;
	l_s_usr_btn_config.vssel= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_usr_btn_config);

	/* SPI0_SS1  */
	l_s_spi_ss1_config.port	= MAX_GPIO_P2_26_SPI0_SS1_PORT_IN;
	l_s_spi_ss1_config.mask	= MAX_GPIO_P2_26_SPI0_SS1_PIN_IN;
	l_s_spi_ss1_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_spi_ss1_config.func	= MXC_GPIO_FUNC_OUT;
	l_s_spi_ss1_config.vssel= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_spi_ss1_config);


#if MAX32690_UI_DEF_ZERO
	/* Prech_En */
	mxc_gpio_cfg_t 			  l_s_prch_en_config;
	l_s_prch_en_config.port	= MAX_GPIO_P1_19_PRECHARGE_EN_PORT_OUT;
	l_s_prch_en_config.mask	= MAX_GPIO_P1_19_PRECHARGE_EN_PIN_OUT;
	l_s_prch_en_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_prch_en_config.func	= MXC_GPIO_FUNC_IN;
	l_s_prch_en_config.vssel= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_prch_en_config);

	/* MCU Status */
	mxc_gpio_cfg_t 			  l_s_mcu_sts_config;
	l_s_mcu_sts_config.port	= MAX_GPIO_P1_7_MCU_STATUS_PORT_OUT;
	l_s_mcu_sts_config.mask	= MAX_GPIO_P1_7_MCU_STATUS_PIN_OUT;
	l_s_mcu_sts_config.pad	= MXC_GPIO_PAD_NONE;
	l_s_mcu_sts_config.func	= MXC_GPIO_FUNC_IN;
	l_s_mcu_sts_config.vssel= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_mcu_sts_config);

	/* FaultB Pin */
	mxc_gpio_cfg_t 			  l_s_faultB_config;
	l_s_faultB_config.port	= MAX_GPIO_P1_14_FAULTB_AFE_PORT_IN;
	l_s_faultB_config.mask	= MAX_GPIO_P1_14_FAULTB_AFE_PIN_IN;
	l_s_faultB_config.pad	= MXC_GPIO_PAD_PULL_UP;
	l_s_faultB_config.func	= MXC_GPIO_FUNC_IN;
	l_s_faultB_config.vssel	= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_faultB_config);

	/* KL_15 */
	mxc_gpio_cfg_t 			  l_s_kl15_config;
	l_s_kl15_config.port	= MAX_GPIO_P1_15_KL_15_MCU_PORT_IN;
	l_s_kl15_config.mask	= MAX_GPIO_P1_15_KL_15_MCU_PIN_IN;
	l_s_kl15_config.pad		= MXC_GPIO_PAD_PULL_UP;
	l_s_kl15_config.func	= MXC_GPIO_FUNC_IN;
	l_s_kl15_config.vssel	= MXC_GPIO_VSSEL_VDDIO;
	MXC_GPIO_Config(&l_s_kl15_config);
#endif
	printf("MAX32690_GPIO->\tMAX32690_GPIO_Init-> GPIO Init: Success\n");
}
/* 	ISO SPI will go to IDLE after 4.3ms. So, Wake isoSPI up from IDlE state and enters the READY state and
	BMS IC will go to sleep state after 2s. So, Wake BMS IC from Sleep state to Standby state					*/
int WakeupIC(mxc_spi_regs_t *spi)
{
	int TOTAL_IC = 1;
	for(int i=0; i<TOTAL_IC; i++)
	{
		/* Wake BMS IC from Sleep state to Standby state */
		spi->ctrl2 |= (MXC_S_SPI_CTRL2_SS_POL_SS0_HIGH);
		MXC_Delay(1000);
		spi->ctrl2 &= ~(MXC_S_SPI_CTRL2_SS_POL_SS0_HIGH);
		MXC_Delay(1000);
		/* Wake isoSPI up from IDlE state and enters the READY state */
		uint8_t a[1] = {0xff};
		SPI_Write(TOTAL_IC, &a[0]);	//Guarantees the isoSPI will be in ready state
		MXC_Delay(200);
	}
		return E_NO_ERROR;
}

void WakeupBMSIC(void)
{
	WakeupIC(SPI);
}
