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
#include "tmr.h"

#include "libtcpm_max25432.h"
mxc_i2c_req_t reqMaster;

#define I2C_FREQ 100000
#define I2C_BYTES 100

#define PORT_GUARANTEED (false)
#define PORT_MANAGED (true)

#define USB_COMM_INCAPABLE (false)
#define USB_COMM_CAPABLE (true)

#define PPS_DISABLED (false)
#define PPS_ENABLED (true)

#define I2C_MASTER MXC_I2C0  //P0.8 & P0.9

#define TIMER_PORT MXC_GPIO0
#define TIMER_PIN MXC_GPIO_PIN_13

#define ALRT_PORT  MXC_GPIO0
#define ALRT_PIN   MXC_GPIO_PIN_11

#define CONT_TIMER MXC_TMR2 // Can be MXC_TMR0 through MXC_TMR5
#define CONT_FREQ 1000

#define PORT1_IDX        0
#define PORT1_ADDR       0x50
#define ALERT1           11
#define PORT_MAX_POWER   100000



void ContinuousTimerHandler(void)
{
 adi_tcpm_timer_ms_irq(); /* 1ms timer lapsed */
 // clear interrupt
 MXC_TMR_ClearFlags(CONT_TIMER);
}

void i2c_write_block(uint8_t i2c_device_addr, uint8_t reg_addr, uint8_t *write_data, uint8_t no_of_bytes) //

{
	reqMaster.i2c  = I2C_MASTER;
	reqMaster.addr = i2c_device_addr;
	uint8_t bytes_len = no_of_bytes + 1; //including the register address

	uint8_t trans_buff[bytes_len];
	trans_buff[0] = reg_addr;            // writing the register address

	for (uint8_t i = 0; i < no_of_bytes;i++)  //for loop for writing the data
	{
		trans_buff[i+1] = write_data[i];
	}
	reqMaster.tx_buf = trans_buff;
	reqMaster.tx_len = sizeof(trans_buff);
	reqMaster.rx_len = 0;                                        //rx_len = 0 for write operation
	MXC_I2C_MasterTransaction(&reqMaster);
}


void i2c_read_block(uint8_t i2c_device_addr, uint8_t reg_addr, uint8_t* Read_data, uint8_t no_of_bytes)
{
	reqMaster.i2c    = I2C_MASTER;
	reqMaster.addr   = i2c_device_addr;
	reqMaster.tx_buf = &reg_addr;                                     //tx_buf is a pointer
	reqMaster.tx_len = sizeof(reg_addr);
	reqMaster.rx_buf = Read_data;                               //
	reqMaster.rx_len = no_of_bytes;                                        //rx_len !=0 fir read operation
	MXC_I2C_MasterTransaction(&reqMaster);
}


/* Platform-specific callback functions */
void reg_read_bytes(uint8_t i2c_device_addr, uint8_t reg_addr, uint8_t *data, uint8_t num_bytes)
{
 (void)i2c_read_block(i2c_device_addr, reg_addr, data, num_bytes);
}
void reg_write_bytes(uint8_t i2c_device_addr, uint8_t reg_addr, uint8_t *data, uint8_t num_bytes)
{
 (void)i2c_write_block(i2c_device_addr, reg_addr, data, num_bytes);
}
bool gpio_read(uint8_t pin_number)
{
// return (gpio_read_input_pin(pin_number) != GPIO_LOW); /* convert to bool */
   return (MXC_GPIO_InGet(ALRT_PORT, ALRT_PIN) != 0);
}
/* Callbacks for adi_tcpm */
struct adi_tcpm_callbacks tcpm_callbacks =
{
 reg_read_bytes,
 reg_write_bytes,
 gpio_read
};
void ContinuousTimer()
{
	mxc_tmr_cfg_t tmr;
	uint32_t periodticks = MXC_TMR_GetPeriod(CONT_TIMER, 1, CONT_FREQ);

	 MXC_TMR_Shutdown(CONT_TIMER);

	tmr.pres = TMR_PRES_1;
	tmr.mode = TMR_MODE_CONTINUOUS;
	tmr.cmp_cnt = periodticks; //SystemCoreClock*(1/interval_time);
	tmr.pol = 0;

	MXC_TMR_Init(CONT_TIMER, &tmr);
	MXC_TMR_Start(CONT_TIMER);

	printf("Continuous timer started.\n\n");
}



int main(void)
{
 uint32_t pdos[7];
 uint8_t nr_pdos;

 //GPIO Configuration
 mxc_gpio_cfg_t one_ms_tmr;
 mxc_gpio_cfg_t alert_int;

 one_ms_tmr.port = TIMER_PORT;
 one_ms_tmr.mask = TIMER_PIN;
 one_ms_tmr.pad = MXC_GPIO_PAD_NONE;
 one_ms_tmr.func = MXC_GPIO_FUNC_OUT;
 one_ms_tmr.vssel = MXC_GPIO_VSSEL_VDDIOH;
 MXC_GPIO_Config(&one_ms_tmr);

 alert_int.port = ALRT_PORT;
 alert_int.mask = ALRT_PIN;
 alert_int.pad = MXC_GPIO_PAD_PULL_UP;
 alert_int.func = MXC_GPIO_FUNC_IN;
 alert_int.vssel = MXC_GPIO_VSSEL_VDDIOH;
 MXC_GPIO_Config(&alert_int);

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

 //Timer Configuration
 MXC_NVIC_SetVector(TMR2_IRQn, ContinuousTimerHandler);
 NVIC_EnableIRQ(TMR2_IRQn);
 ContinuousTimer();


 /* Initialize general peripherals here */
// system_init();
 /* Setup TCPM callbacks */
 adi_tcpm_set_callbacks(&tcpm_callbacks);
 /* Setup TCPC ports */
 adi_tcpm_set_port(PORT1_IDX, PORT1_ADDR, ALERT1, PORT_MAX_POWER, PORT_GUARANTEED);
 /* Setup TCPC port capabilities (PDOs) */
 adi_tcpm_build_pdos(pdos, &nr_pdos, PORT_MAX_POWER, USB_COMM_INCAPABLE, PPS_ENABLED);
 adi_tcpm_update_source_caps(PORT1_IDX, pdos, nr_pdos);


     while (1)
	 {
	  /* Call TCPM state manager */
      adi_tcpm_state_manager();
	 }
 return 0;
}




