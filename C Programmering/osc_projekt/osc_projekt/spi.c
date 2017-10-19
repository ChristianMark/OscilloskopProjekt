/*
 * spi.c
 *
 * Created: 6/10/2017 13:54:52
 *  Author: Christian
 */ 
/*
 * SPI_master.c
 */ 

#include <avr/io.h>
#include "Header.h"

//PORTE
#define DDRx DDRB
#define SS   0
#define SCK  1
#define MOSI 2
#define MISO 3

/*
unsigned char addrTest = 0x02;
unsigned char dataTest = 0x55;
*/

unsigned char SPI_MasterReceive(void){
	while(!(SPSR &(1<<SPIF)));
	return SPDR;
}

void SPI_MasterInit(){
	
	DDRB |=(1<<DDB2) | (1<<DDB1) | (1<<DDB0); // Sætter data direction registeret til output på pin 51, 52 og 53 (SPI benene).
	SPCR |=(1<<SPE)|(1<<MSTR)|(1<<CPOL);  //side 202 clock polarity: Cpol = 1, SCK er høj når idle. Leading Edge er på "falling"
	SPCR |=(1<<SPR1); SPSR |=(1<<SPI2X); //500kHz
	PORTB |=(1<<PB0);
}
//SCK	Grøn	Pin 52 | B2
//MISO	Ingen	Pin 50 | B0
//MOSI	Hvid	Pin 51 | B1
//SS	Rød		Pin 53 | B3


void SPI_MasterTransmit(unsigned char data) {
	PORTB &=~(1<<PB0);
	SPDR = data;
	while (!(SPSR &(1<<SPIF)));
	data = SPDR;	
	PORTB |=(1<<PB0);
}

unsigned char SPI_send(unsigned char addr, unsigned char data){
	unsigned char syncbyte = 69;
	unsigned char checkbyte = addr ^ data;
	
	put_char(syncbyte);
	put_char(addr);
	put_char(data);
	put_char(checkbyte);
	
	SPI_MasterTransmit(syncbyte);
	SPI_MasterTransmit(addr);
	SPI_MasterTransmit(data);
	SPI_MasterTransmit(checkbyte);
	return 0;
}



