// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/29/2025         
// Assignment: Lab 4
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

void initSwitch() {
    DDRD &= ~(1 << DDD0); // Set PD0 as input (INT0)
    PORTD |= (1 << PORTD0); // Enable pull-up resistor on PD0
    EICRA |= (1 << ISC00); // Trigger INT0 on any logical change
    EIMSK |= (1 << INT0); // Enable INT0 interrupt
}
