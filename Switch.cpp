// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/29/2025         
// Assignment: Lab 5
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

void initSwitch() {
    DDRE &= ~(1 << DDE0); // Set PE0 as input (INT0)
    PORTE |= (1 << PORTE0); // Enable pull-up resistor on PE0
    PCICR |= (1<<PCIE1); 
    PCMSK1 |= (1<<PCINT8);
}
