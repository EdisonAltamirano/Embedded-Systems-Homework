/*
 * spi.h
 *
 * Created: 5/11/2022 6:11:12 PM
 *  Author: riky_
 */ 

#ifndef SPI_H_
#define SPI_H_
#define BAUDRATE 9600
void spiInit( void );
uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff );
void initCycles(void);
uint8_t spiSend( uint8_t data );

#endif /* SPI_H_ */
