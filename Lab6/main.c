/*
 * RTC_i2C.c
 *
 * Created: 26/09/2019 10:05:21 p. m.
 * Author : Asus X556U
 */ 

#include "sam.h"
#include "myprintf.h"
#include "stdbool.h"
#include "RTCControl.h"

#define SLAVE_ADDR 0x68u
#define BUF_SIZE 4
#define DAY_ADDR 0x03u

#define RTC_ADDR 0x68u
#define START_PTR 0x00u


// Control variables for data transfer and reception
volatile uint32_t i;
//uint8_t tx_buf[BUF_SIZE] = {DAY_ADDR, 0x03u, 0x22u, 0x04u};
//uint8_t rx_buf[BUF_SIZE];

uint8_t curr_date[8] = {START_PTR, 0x00u,0x28u,0x06u,0x00u,0x04u,0x05u,0x22u};
uint8_t rec_date[7] = {0};

void UARTInit(void);

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	UARTInit();
	
	//char buff[31]; buff[30]= '\0';
	//mysnprintf(buff, sizeof buff, "testing %d %d %07d", 1, 2, 3);
	//myprintf("\nBuff=%s",buff);
	//mysnprintf(buff, sizeof buff, "faster %s %ccheaper%c", "and", 34, 34);
	//myprintf("\nBuff=%s",buff);
	//mysnprintf(buff, sizeof buff, "%x %% %X", 0xdeadf00d, 0xdeadf00d);
	//myprintf("\nBuff=%s",buff);
	//mysnprintf(buff, sizeof buff, "%09d%09d%09d%09d%09d", 1, 2, 3, 4, 5);
	//myprintf("\nBuff=%s",buff);
	//mysnprintf(buff, sizeof buff, "%d %u %d %u", 50, 50, -50, -50);
	//myprintf("\nBuff=%s",buff);
	myprintf("\nHello World");
	
	initialize();
	timers();
	seg_type seg;
	min_type min;
	hour_type hour;
	date_type date;
	month_type month;
	year_type year;
	sendI2CDataArray(RTC_ADDR,START_PTR,curr_date,8);
	while(1){
		
		relocatePointer(RTC_ADDR,START_PTR);
		receiveI2CDataArray(RTC_ADDR,START_PTR,rec_date,8);
		relocatePointer(RTC_ADDR,START_PTR);
		seg.reg = rec_date[0];
		min.reg = rec_date[1];
		hour.reg = rec_date[2];
		date.reg = rec_date[4];
		month.reg = rec_date[5];
		year.reg = rec_date[6];

		while((TC3->COUNT16.INTFLAG.reg & 0x01) == 0);
		TC3->COUNT16.INTFLAG.reg &= 0xF7;
		myprintf("%d%d/", date.field.dec_date, date.field.un_date);
		myprintf("%d%d/", month.field.dec_month, month.field.un_month);
		myprintf("%d%d ", year.field.dec_year, year.field.un_year);
		
		myprintf("%d%d:", hour.field.dec_hour, hour.field.un_hour);
		myprintf("%d%d:", min.field.dec_min, min.field.un_min);
		myprintf("%d%d\n", seg.field.dec_seg, seg.field.un_seg);

	}
	
	// Turn off the I2C
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0x0u;
	
}

void UARTInit(void) {
	/* Initialize the SAM system */
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration*/
	PORT->Group[0].DIR.reg |= (1 << 10);                  /* Pin 10 configured as output */
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	
	/*PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[5].reg |= 0x02;                   /* Selecting peripheral function C */
	PORT->Group[0].PMUX[5].reg |= 0x20;                   /* Selecting peripheral function C */
	
	/* APBCMASK */
	//PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;			  /* SERCOM 0 enable*/
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	/*GCLK configuration for sercom0 module: using generic clock generator 0, ID for sercom0, enable GCLK*/

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	
	/* configure SERCOM0 module for UART as Standard Frame, 8 Bit size, No parity, BAUDRATE:9600*/

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);
	
	uint64_t br = (uint64_t)65536 * (8000000 - 16 * 9600) / 8000000;
	
	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

void timers(){
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
	GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3;      // Use GCLK0 for TC3
	
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;                  // Reset TC3
	while (TC3->COUNT16.CTRLA.bit.SWRST);                     // Wait for completion

	PORT->Group[0].PINCFG[18].bit.PMUXEN = PORT_PINCFG_PMUXEN;// Enable PMUX for PA18
	PORT->Group[0].PMUX[9].bit.PMUXE = PORT_PMUX_PMUXE_E;     // Assign PA18 to TC3
	PM->APBCMASK.reg = PM->APBCMASK.reg | PM_APBCMASK_TC3;    // Enable clock for TC3
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |          // 16-bit mode
	TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1024;     // MFRQ mode, / 1024
	
	TC3->COUNT16.CC[0].reg = 7812;                           // Divide clock by 46875
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);     // Synchronize

	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;                // Enable TC3
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);     // Synchronize
	
}