// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/27/2025         
// Assignment: Lab 4
//----------------------------------------------------------------------//

#include "timer.h"

void initTimer1() {
    // Configure Timer1 for 1-second intervals
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

void delaySec(unsigned int delay) {
    // Reset Timer1 settings
    TCCR1B = 0;
    
    // Configure for 1-second intervals with 1024 prescaler
    TCCR1B = (1 << CS12) | (1 << CS10);
    OCR1A = 15624;

    for(unsigned int i = 0; i < delay; i++) {
        // Reset counter
        TCNT1 = 0;
        // Clear output compare flag
        TIFR1 = (1 << OCF1A);
        // Wait until compare match occurs
        while(!(TIFR1 & (1 << OCF1A)));
    }
    
    // Stop timer by clearing prescaler
    TCCR1B = 0;
}

void initTimer0(){
    // Clear all WGM bits first
    TCCR0A &= ~((1 << WGM00) | (1 << WGM01));
    TCCR0B &= ~(1 << WGM02);
    // Set WGM01 for CTC mode
    TCCR0A |= (1 << WGM01);
}

void delayMs(unsigned int delay){
    // Set compare match value for 1ms timing
    OCR0A = 249;
    // Set prescaler to 64 
    TCCR0B = (1 << CS01) | (1 << CS00);
    for(unsigned int i = 0; i < delay; i++){
        // Reset counter
        TCNT0 = 0;
        // Clear output compare flag
        TIFR0 = (1 << OCF0A);
        // Wait until compare match occurs
        while((TIFR0 & (1 << OCF0A)) == 0);
    }
    // Stop timer by clearing prescaler
    TCCR0B = 0;
}