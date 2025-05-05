// Author: Abril Torres      
// Date:  April 29th, 2025 
// Assignment:     Final project 
//----------------------------------------------------------------------//

#include "pwm.h"
#include <avr/io.h>

void initPWMTimer3(){ 
    //Fast mode 15 - 1111
    //Motor A right 
    DDRB |= (1<<PB0) | (1<<PB1);  // IN1, IN2 // Pin B0 is output - OC3A, Pin B1 is output - OC3A

    //PWM control 
    DDRE |= (1 << PE3); //Pin E4-A is output - OC3A 

    // Timer 3 - Fast mode 10 - 111 
    TCCR3A |= (1<<WGM30); 
    TCCR3A |= (1<<WGM31);  
    TCCR3B |= (1<<WGM32);  

    // Need non-inverted PWM output so 01 for Compare Output Mode, Fast PWM 
    // Still working on timer 3 
    TCCR3A |= (1 << COM3A1);  // Right motor on PE4
 
    // Prescaler = 1 100 
    TCCR3B |= (1<<CS30);
    TCCR3B &= ~(1<<CS31); 
    TCCR3B &= ~(1<<CS32);

     // Set initial duty cycle to 
     OCR3A = 800;  // Right motor
     
}

    void initPWMTimer4(){
        //Motor B left 
    DDRB |= (1<<PB2) | (1<<PB3);   // IN3, IN4

    DDRH |= (1 << PH3); //Pin H4 is output - OC4A

    // Timer 4 - Fast mode 10 - 111
    TCCR4A |= (1<<WGM40); 
    TCCR4A |= (1<<WGM41);  
    TCCR4B |= (1<<WGM42);  

    // Need non-inverted PWM output so 01 for Compare Output Mode, Fast PWM 
    TCCR4A |= (1 << COM4A1);  // Right motor on PE4

    // Prescaler = 1 100 
    TCCR4B |= (1<<CS40);
    TCCR4B &= ~(1<<CS41); 
    TCCR4B &= ~(1<<CS42);

     // Set initial duty cycle to 50%
     OCR4A = 800;  // left motor

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
    PORTA &= ~(1 << 2);
    PORTA |= (1 << 3);
}

void setDirectionBackward() {
    // Left motor backward (IN1=LOW, IN2=HIGH)
    PORTA &= ~(1 << PA0);
    PORTA |= (1 << PA1);
    OCR4A = 512;  // left motor
    // Right motor backward (IN3=HIGH, IN4=LOW)
    PORTA |= (1 << PA2);
    PORTA &= ~(1 << PA3);
    OCR3A = 512;  // Right motor
}

void setDirectionLeft() {
    // Left motor backward
    PORTA &= ~(1 << PA0);
    PORTA |= (1 << PA1);

    // Right motor forward
    PORTA &= ~(1 << PA2);
    PORTA |= (1 << PA3);
}

void setDirectionRight() {
    // Left motor forward
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
    OCR4A = 512;  // left motor

    // Right motor backward
    PORTA |= (1 << PA2);
    PORTA &= ~(1 << PA3);
    OCR3A = 512;  // Right motor
}

void stopMotors() {
    OCR3B = 0;
    OCR3C = 0;
    PORTA &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Set all direction pins to low
}
