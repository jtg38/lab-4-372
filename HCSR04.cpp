// HCSR04.cpp

#include "HCSR04.h"
#include "timer.h"
#include <avr/io.h>        
#include <avr/interrupt.h> 
#include <Arduino.h>

void HCSR04_init() {
    // Configure Pin 9 (PH6) as Output (Trigger)
    DDRH |= (1 << PH6);

    // Configure Pin 10 (PB4) as Input (Echo)
    DDRB &= ~(1 << PB4);

    // Set Trigger Pin 9 (PH6) Low initially
    PORTH &= ~(1 << PH6);
    initTimer1();
}


// Measures the distance using pins 9 & 10. Uses register access for trigger pulse.
float HCSR04_distance() {
    // Set Pin 9 LOW
    PORTH &= ~(1 << PH6);
    delayUs(2); // Wait for the echo pin to go LOW (start of pulse)
    // Set Pin 9 HIGH
    PORTH |= (1 << PH6);
    delayUs(10); // Trigger pulse for 10us 
    // Set Pin 9 LOW
    PORTH &= ~(1 << PH6);
	delayUs(2); // Wait for the echo pin to go HIGH (start of pulse)
    cli();
    int duration_ticks = 0; 
    // Use Timer1 briefly to timeout waiting for the rising edge
    TCNT1 = 0;
    // Start Timer1 with prescaler 8
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Clear other CS bits
    TCCR1B |= (1 << CS11);                  // Set CS11 for prescaler 8
    while (!(PINB   & (1 << PB4))) 
    TCNT1 = 0; // Reset Timer1 counter now that pulse has started
    // Wait for the echo pin to go LOW (end of pulse)
    while (PINB & (1 << PB4)) 
    duration_ticks = TCNT1; // Read timer count
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Stop Timer1
    sei();
    // duration (us) = ticks / (ticks/us)
    float duration = (float)duration_ticks / 2; // Convert to microseconds (2 ticks per microsecond)
    // Calculate Distance 
    float distance = duration / 58.8235;
    return distance;
}