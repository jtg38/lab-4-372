// Author: Abril Torres      
// Date:  March 30, 2025    
// Assignment:     Lab 4
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

void initPWMTimer3(){ 
    DDRB |= (1<<PB0); // Pin B0 is output - OC3A
    DDRB |= (1<<PB1); // Pin B1 is output - OC3A

    DDRE |= (1<<PE3); //Pin E3 is output - OC3B 

    // Timer 3 - need 10 bit mode so 0111 
    // Set to Fast PWM Mode, 10 bit 
    TCCR3A |= (1<<COM3A1); // Set WGM30 to 1
    TCCR3A |= (1<<WGM30); 
    TCCR3A |= (1<<WGM31);  
    TCCR3B |= (1<<WGM32);

    // Need non-inverted PWM output so 01 for Compare Output Mode, Fast PWM 
    // Still working on timer 3 
    TCCR3A |= (1<<COM3A1); 

    // Prescaler = 8
    TCCR3B &= ~(1<<CS30);
    TCCR3B |= (1<<CS31); 
    TCCR3B &= ~(1<<CS32);

    OCR3A = 0; // Set initial duty cycle to 0%
}

  //frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
  // Prescaler = 8
  // TOP value = 0x3FF = 1023 
  // PWM frequency from calculation = 1.953 kHz

// the last thing is to set the duty cycle.     
// duty cycle is set by dividing output compare OCR3B value by 1 + TOP value
// the top value is (1 + ICR1) = 1024
//  calculate OCR3B value => OCR3B = duty cycle(fractional number) * (1 + TOP) 
// we want a duty cycle = 60%
// OCR3B = need to change according to adc value 


void changeDutyCycle(int adcInput){
    // Map ADC (0–1023) to PWM duty cycle (0–1023 for 10-bit)
    if (adcInput < 511) {
        PORTB |= (1<<PB0); // Set to 1 low
        PORTB &= ~(1<<PB1); // Set to 0 high
        OCR3A = 1023 - (2*adcInput); // Scale to 0-1023
    }
    else if (adcInput > 512) {
        PORTB &= ~(1<<PB0); // Set to 0 low
        PORTB |= (1<<PB1); // Set to 1 high
        OCR3A = (2*(adcInput-512)); // Scale to 0-1023
    }
    else {
        OCR3A = 0; // Scale to 0-1023
        PORTB &= ~(1<<PB1); // Set to 0 high
        PORTB &= ~(1<<PB0); // Set to 0 low
    }

}