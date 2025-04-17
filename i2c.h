#ifndef i2c_H
#define i2c_H

void InitI2C(void);
void StartI2C_Trans(unsigned char SLA);
void StopI2C_Trans();
void Write(unsigned char data);
void Read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char Read_data();

#endif
