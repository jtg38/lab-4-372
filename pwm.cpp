// Author: Abril Torres      
// Date:  April 29th, 2025 
// Assignment:     Final project 
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

void initPWMTimer3(){ 
    //Fast mode 15 - 1111
    //Motor A 
    DDRB |= (1<<PB0) | (1<<PB1);  // IN1, IN2 // Pin B0 is output - OC3A, Pin B1 is output - OC3A

    //Motor B 
    DDRB |= (1<<PB2) | (1<<PB3);   // IN3, IN4

    //PWM control 
    DDRE |= (1 << PE4) | (1 << PE5); //Pin E4-A and E5-B is output - OC3B 

    // Timer 3 - Fast mode 15 - 1111  
    TCCR3A |= (1<<WGM30); 
    TCCR3A |= (1<<WGM31);  
    TCCR3B |= (1<<WGM32);  
    TCCR3B |= (1<<WGM33);

    // Need non-inverted PWM output so 01 for Compare Output Mode, Fast PWM 
    // Still working on timer 3 
    TCCR3A |= (1 << COM3B1);  // Right motor on PE4
    TCCR3A |= (1 << COM3C1);  // Left motor on PE5

    // Prescaler = 1 100 
    TCCR3B |= (1<<CS30);
    TCCR3B &= ~(1<<CS31); 
    TCCR3B &= ~(1<<CS32);

     // Set TOP value for 400 Hz frequency
     ICR3 = 39999;

     // Set initial duty cycle to 50%
     OCR3B = 20000;  // Right motor
     OCR3C = 20000;  // Left motor379

}

    // Initialize Direction Pins on PA0–PA3 to outputs 
    void initMotorDirections() {
        DDRA |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    }
  //frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
 

    // Moving forward 
  void setDirectionForward() {
    // Left motor forward (PA0 HIGH, PA1 LOW)
    PORTA |= (1 << 0);
    PORTA &= ~(1 << 1);

    // Right motor forward (PA2 HIGH, PA3 LOW)
    PORTA |= (1 << 2);
    PORTA &= ~(1 << 3);
}

void stopMotors() {
    OCR3B = 0;
    OCR3C = 0;
}



