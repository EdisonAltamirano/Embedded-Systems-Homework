/*
 * Laboratory7.c
 *
 * Created: 5/11/2022 6:10:10 PM
 * Author : riky_
 */ 


#include "sam.h"
#include "spi.h"
#include "myprintf.h"
#include "uart.h"
#define RXBUFSIZE 0x400
#define LENGTH_R1 0x03
#define LENGTH_R7 0x07
#define BS 512
void initCycles(void);
#define SIZE_SD_CMD 0x06
#define kCMD00 0x40
#define kCMD08 0x48
#define kCMD55 0x77
#define kCMD41 0x69
const uint8_t CMD00[SIZE_SD_CMD] ={0x40, 0x00, 0x00, 0x00, 0x00, 0x95}; 
const uint8_t CMD08[SIZE_SD_CMD] ={0x48,0x00,0x00,0x01,0xAA,0x87}; 
uint8_t CMD17[SIZE_SD_CMD] ={0x51, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t CMD172[SIZE_SD_CMD] ={0x51, 0x00, 0x00, 0x08, 0x00, 0x01};
const uint8_t CMD18[SIZE_SD_CMD] ={0x52, 0x00, 0x00, 0x00, 0x00, 0x01}; 
const uint8_t CMD55[SIZE_SD_CMD] ={0x77, 0x00, 0x00, 0x00, 0x00, 0x65}; 
const uint8_t CMD41[SIZE_SD_CMD] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};
uint8_t RxBuffer[RXBUFSIZE];
int main(void)
{
	uint32_t temp = 0xFF;
	/* Initialize the SAM system */
	SystemInit();
	UARTInit();
	spiInit();
	initCycles();
	//myprintf("\n");

	while(RxBuffer[1]!=1){
		myprintf("\nCM0: ");
		spiXchg( CMD00, SIZE_SD_CMD, RxBuffer ); /* reset instruction */		
	}
	myprintf("\nCM8: ");
	spiXchg( CMD08, SIZE_SD_CMD, RxBuffer ); /* reset instruction */
	
	if(RxBuffer[1] & 0x04 == 1){ // Ilegal
		myprintf("\n Illegal ");
		return 0;
	}else{
		if((RxBuffer[5] == 0xaa) && ((RxBuffer[4] & 0x0F) == 0x01)){
			while(RxBuffer[1] != 0x00){
				myprintf("\nCM55: ");
				spiXchg( CMD55, SIZE_SD_CMD, RxBuffer ); 
				myprintf("\nCM41: ");
				spiXchg( CMD41, SIZE_SD_CMD, RxBuffer ); 
			}
			uint32_t lba = 0x00000000;
			for(int i = 0; i<5; i++){
				myprintf("\nCM17 - Address: %x \n",lba);
				rcvr_datablock(CMD17,lba,RxBuffer,BS);
				lba += 0x200;
			}
			

		}else{
			myprintf("\n Voltage or check pattern are not correct.\n");
		}
		
		
	}
	//Write your code here
	myprintf("\nDone");
}
void initCycles(void){
	uint32_t i;
	REG_PORT_OUTSET0 = PORT_PA18;
	for(i=0;i<76;i++)
	spiSend(0xFF);
}
//ADD spiXchg FUNCTION TO THE “spi.c” FILE
uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	uint8_t temp_cmd = send_buff[0];
	REG_PORT_OUTCLR0 = PORT_PA18;
	for(i=0; i< bc; i++) {
		temp = spiSend(*(send_buff++));
		//myprintf(" %x", temp);
	}
	switch(temp_cmd) {
		case kCMD00 :
		for(i=0; i<LENGTH_R1; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
			receive_buff[i]=temp;
		}
		break;
		case kCMD08 :
		for(i=0; i<LENGTH_R7; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
			receive_buff[i]=temp;
		}
		break;
		case kCMD41 :
		for(i=0; i<LENGTH_R1-1; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
			receive_buff[i]=temp;
		}
		spiSend(0xFF);
		break;
		case kCMD55 :
		for(i=0; i<LENGTH_R1; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
			receive_buff[i]=temp;
		}
		break;
		default :
		myprintf("\n Error in CMD");
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	return(temp);
}

void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs ) {  
	uint8_t temp = 0xFF; 
	uint32_t i; 
	REG_PORT_OUTCLR0 = PORT_PA18; 
	myprintf("\n\n"); 
	temp = send_buff[0]; 
	temp = spiSend(temp); 
	myprintf(" %x", temp); 
	temp = ((uint8_t*)&lba)[3]; 
	temp = spiSend(temp); 
	temp = ((uint8_t*)&lba)[2]; 
	temp = spiSend(temp); 
	temp = ((uint8_t*)&lba)[1]; 
	temp = spiSend(temp); 
	temp = ((uint8_t*)&lba)[0]; 
	temp = spiSend(temp); 
	myprintf(" %x", temp); 
	// Complete the code that is missing 
	temp = send_buff[5]; 
	temp = spiSend(temp); 
	myprintf(" %x", temp); 
	// Reading to find the beginning of the sector 
	temp = spiSend(0xFF); 
	while(temp != 0xFE){ 
	temp = spiSend(0xFF); 
	myprintf(" %x", temp); 
	} 
	// Receiving the memory sector/block 
	myprintf("\n\n"); 
	for(i=0; i< bs; i++) { 
		while(SERCOM1->SPI.INTFLAG.bit.DRE == 0); 
		SERCOM1->SPI.DATA.reg = 0xFF; 
		while(SERCOM1->SPI.INTFLAG.bit.TXC == 0); 
		while(SERCOM1->SPI.INTFLAG.bit.RXC == 0); 
		temp = SERCOM1->SPI.DATA.reg; 
		*(receive_buff++) = temp;
		myprintf(" %x", temp); 
    } 
 REG_PORT_OUTSET0 = PORT_PA18; 
 myprintf("\n\n"); 
}


