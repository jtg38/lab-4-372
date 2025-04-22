#include <avr/interrupt.h> //enables interrupts 
#include <avr/io.h>

//spi uses port B direction reg 
#define DDR_SPI DDRB  
//spi pins 
#define DD_SS DDB0  
#define DD_SCK DDB1 
#define DD_MOSI DDB2 
//spi output port
#define SPI_PORT PORTB 
//slave select pin 
#define SPI_SS_BIT PORTB0 
//wait until transmission is complete
#define wait_for_complete while(!(SPSR & (1 << SPIF)));

//initialize master 
void SPI_MASTER_Init() {
    //MOSI, SCK=PB1 , SS=PB0 as output pins 
    DDR_SPI = (1 << DD_MOSI)| (1 << DD_SCK) | (1 << DD_SS);
    //set ss pin high - slave inactive 
    SPI_PORT  |= (1 << SPI_SS_BIT);
    //SPE-spi enabled, MSTR-in master mode, CPOL-clk polarity to idle high, CPHA-clk phase to sampling on rising edge, SPR1 AND SPR0-set clk speed to lowest speed = fosc/128
    SPCR  |= (1 <<SPE)| (1 << MSTR) | (1 << CPOL) | (1 << CPHA ) | (1 << SPR1) | (1 << SPR0);
    }

//write/execute function 
//fn to send 2 bytes - command and data associated with that command 
void write_execute(unsigned char CMD, unsigned char data) {
    //pull SS low to select the slave device
    SPI_PORT &= ~(1 << SPI_SS_BIT);     
    //loads command byte into SPI data reg 
    SPDR = CMD;  
    //waits until command has been fully transmitted
    wait_for_complete;    
    //loads data byte into SPI data reg 
    SPDR = data;     
    //wait until data byte has been transmitted 
    wait_for_complete;                  
    //pulls ss again to end communication 
    SPI_PORT |= (1 << SPI_SS_BIT);     
}
