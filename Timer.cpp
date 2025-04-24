// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/27/2025         
// Assignment: Lab 4
//----------------------------------------------------------------------//

#include "timer.h"

void initTimer1() {
    // Clear all WGM bits first
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B |= (1 << WGM12);
    // Set WGM01 for CTC mode
    TCCR1B &= ~(1 << WGM13);
}

void delayMs(unsigned int delay) {
    // Configure for 1ms intervals with 64 prescaler
    TCCR1B = (1 << CS11) | (1 << CS10); 
    TCCR1B &= ~(1 << CS12);
    OCR1A = 249;

    for(unsigned int i = 0; i < delay; i++) {
        // Reset counter
        TCNT1 = 0;
        // Clear output compare flag
        TIFR1 = (1 << OCF1A);
        // Wait until compare match occurs
        while(!(TIFR1 & (1 << OCF1A)));
    }
    
    TCCR1B &= ~(1 << CS11) | (1 << CS10); 
    // Stop timer by clearing prescaler
    TCCR1B = 0;
}
