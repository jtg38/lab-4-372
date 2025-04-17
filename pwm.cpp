// Author: Abril Torres      
// Date:  April 17th, 2025    
// Assignment:     Lab 5
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

//Use timer 4 for calculating frequency and duty cycle 
void initPWMTimer4(){ 
    //We need fast PWM mode 15 for variable frequency 
    //Set to 1111 for PWM mode 15 
    TCCR4A |= (1<<WGM40); 
    TCCR4A |= (1<<WGM41); 
    TCCR4B |= (1<<WGM42);
    TCCR4B |= (1<<WGM43);

    // Set to Fast PWM Mode non-inverting mode  - 10 
    TCCR4A &= ~(1<<COM4C0); 
    TCCR4A |= (1<<COM4C1); 

    //We're using prescaler of 1 - 001 
    TCCR4B |= (1<<CS40); 
    TCCR4B &= ~(1<<CS41); 
    TCCR4B &= ~(1<<CS42); 

    DDRH |= (1<<DDH5); // Set pin 8 on board to output -0C4C, PORTH5
    //Set direction to pin 8 of port H5 as output 
}

  // frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
  // Variable frequency Function to change output sound frequency 
void change_frequency(unsigned int freq_val) {
    OCR4A = 16000000/freq_val; //TOP value 
} 
// duty cycle is set by dividing output compare OCR4C value by 1 + TOP value
// the top value is (1 + ICR1) = 1024
// calculate OCR4C value => OCR4C = duty cycle(fractional number) * (1 + TOP)
void change_duty_cycle(double duty_val) {
    OCR4C = OCR4A * duty_val; //Compare matche value 
}



