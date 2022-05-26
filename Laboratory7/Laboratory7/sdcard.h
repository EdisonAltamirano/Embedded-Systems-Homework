/*
 * sdcard.h
 *
 * Created: 5/11/2022 7:38:48 PM
 *  Author: riky_
 */ 


#ifndef SDCARD_H_
#define SDCARD_H_
uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff );
void initCycles(void);




#endif /* SDCARD_H_ */