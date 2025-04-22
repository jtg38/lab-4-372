#include <avr/interrupt.h>
#include <avr/io.h>

#define DDR_SPI DDRB
#define DD_SS DDB0  
#define DD_SCK DDB1 
#define DD_MOSI DDB2 
#define SPI_PORT PORTB 
#define SPI_SS_BIT PORTB0 

#define wait_for_complete while(!(SPSR & (1 << SPIF)));


void SPI_MASTER_Init() {

    DDR_SPI = (1 << DD_MOSI)| (1 << DD_SCK) | (1 << DD_SS);
    SPI_PORT  |= (1 << SPI_SS_BIT);
    SPCR  |= (1 <<SPE)| (1 << MSTR) | (1 << CPOL) | (1 << CPHA ) | (1 << SPR1) | (1 << SPR0);

    }

void write_execute(unsigned char CMD, unsigned char data) {
    SPI_PORT &= ~(1 << SPI_SS_BIT);     
    SPDR = CMD;                         
    wait_for_complete;                  
    SPDR = data;                       
    wait_for_complete;                  
    SPI_PORT |= (1 << SPI_SS_BIT);     
}
