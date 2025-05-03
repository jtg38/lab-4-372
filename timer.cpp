#include "timer.h"

// Set to Normal mode (TOP = 0xFFFF). Prescaler will be set when measuring. 
void initTimer1(){ // (16 bits for duration measurement)
    // Set Timer1 to Normal mode (WGM10=0, WGM11=0, WGM12=0, WGM13=0)
	TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13)); 
    // Ensure timer is stopped initially
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void delayUs(unsigned int delay){
    // Save current Timer1 settings
    int oldTCCR1A = TCCR1A;
    int oldTCCR1B = TCCR1B;

    // Configure Timer1 for CTC mode (WGM12 = 1) for this delay function
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11)); // WGM10=0, WGM11=0
    TCCR1B |= (1 << WGM12);                  // WGM12=1
    TCCR1B &= ~(1 << WGM13);                 // WGM13=0

    OCR1A = 2; // Using original value

    // Set prescaler to 8 (CS11 only) and start timer
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Clear other CS bits
    TCCR1B |= (1 << CS11);

    for(unsigned int i = 0; i < delay; i++){
        // Clear counter
        TCNT1 = 0;
        // Clear output compare match flag by writing a 1
        TIFR1 = (1 << OCF1A);
        // Wait for compare match flag to be set
        while(!(TIFR1 & (1 << OCF1A)));
    }

    // Stop timer by clearing prescaler bits
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    TCCR1A = oldTCCR1A;
    TCCR1B = oldTCCR1B;
}

void initTimer0(){ // (8 bits)
    // Clear all WGM bits first
    TCCR0A &= ~((1 << WGM00) | (1 << WGM01));
    TCCR0B &= ~(1 << WGM02);
    // Set WGM01 for CTC mode
    TCCR0A |= (1 << WGM01);
    // Ensure timer is stopped initially
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

void delayMs(unsigned int delay){
    // Save current Timer0 settings
    int oldTCCR0B = TCCR0B;

    // Set compare match value for 1ms timing @ 16MHz
    OCR0A = 249;

    // Set prescaler to 64 (CS01 and CS00 bits) and start timer
    TCCR0B &= ~(1 << CS02); // Clear CS02
    TCCR0B |= (1 << CS01) | (1 << CS00); // Set CS01 and CS00

    for(unsigned int i = 0; i < delay; i++){
        // Reset counter
        TCNT0 = 0;
        // Clear output compare flag by writing a 1
        TIFR0 = (1 << OCF0A);
        // Wait until compare match occurs
        while(!(TIFR0 & (1 << OCF0A)));
    }
    // Stop timer by clearing prescaler bits
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

    TCCR0B = oldTCCR0B;
}