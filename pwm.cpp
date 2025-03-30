// Author: Abril Torres      
// Date:  March 30, 2025    
// Assignment:     Lab 4
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

void initPWMTimer3(){ 

    DDRE |= (1<<PE4); //Pin E4 is output - OC3B 

    // Timer 3 - need 10 bit mode so 0111 
    // Set to Fast PWM Mode, 10 bit 
    TCCR3A |= (1<<WGM30); 
    TCCR3A |= (1<<WGM31);  
    TCCR3B |= (1<<WGM32);
    TCCR3B &= ~(1<<WGM33); 

    // Need non-inverted PWM output so 01 for Compare Output Mode, Fast PWM 
    // Still working on timer 3 
    TCCR3A &= ~(1<<COM3B0);
    TCCR3A |= (1<<COM3B1); 

    // Prescaler = 8
    TCCR3B &= ~(1<<CS30);
    TCCR3B |= (1<<CS31); 
    TCCR3B &= ~(1<<CS32);
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


void changeDutyCycle(unsigned int adcInput){
    OCR3B = adcInput; // Compare to ADC value, if 0, 0% duty cycle; if 255, 100% duty cycle
}

// H bridge motor control with PB0 and PB1 
// Since the motor is connected between two outputs, switching the voltage levels (0 and 1) reverses the current flow, changing the direction of rotation
void motorDirection(unsigned char direction){
    if(direction == 0){
        // Forward direction
        PORTB &= ~(1<<PB0); // Set to 0 (clear)
        PORTB |= (1<<PB1); // Set to 1 (set)
    }
    else{
        // Backward direction
        PORTB |= (1<<PB0); // Set to 1 (set)
        PORTB &= ~(1<<PB1); // Set to 0 (clear)
    }
}
