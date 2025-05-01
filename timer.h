// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/27/2025         
// Assignment: Lab 4
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer1();
void delaySec(unsigned int delay);
void initTimer0();
void delayMs(unsigned int delay); 

#endif
