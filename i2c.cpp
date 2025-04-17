#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"
#define wait_for_completion while(!(TWCR & (1 <<TWINT)));


void InitI2C(void) {
    PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
    TWSR |= (1 << TWPS0);  // prescaler power = 1
    TWSR &= ~(1 << TWPS1); // prescaler power = 1 
    // 10k Hz at 16MHz
    TWBR = 0xC6; 
    // enable the TWI interface
    TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
}
void StartI2C_Trans(unsigned char SLA){
    // set the start condition
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA));
    wait_for_completion;
    // send the address
    TWDR = (SLA << 1); // shift SLA left by 1 to make room for R/W bit
    TWCR = ((1 << TWEN) | (1 << TWINT)); // trigger I2C action
    wait_for_completion;
}
void StopI2C_Trans(){
    TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTO)); // set stop condition
    wait_for_completion;
}
void Write(unsigned char data){
    TWDR = data; // load data into the data register
    TWCR = ((1 << TWEN) | (1 << TWINT)); // trigger I2C action
    wait_for_completion;
}
void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
    // this function sets up reading from SLA at the SLA MEMADDRESS
    StartI2C_Trans(SLA);
    Write(MEMADDRESS); // send the memory address to read from
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
    wait_for_completion;
    TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
    TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
    wait_for_completion;
    TWCR = (1<< TWINT) | (1 << TWEN); // master can send a nack now
    wait_for_completion;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
    // after this function is executed the TWDR register has the data from SLA that Master wants to read
}
unsigned char Read_data() {
    return TWDR; // return the data in the TWDR register
}
