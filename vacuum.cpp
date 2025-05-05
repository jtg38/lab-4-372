#include <avr/io.h>

void initPWMTimer5(){

    DDRJ |= (1 << PJ0) | (1 << PJ1);  // Set PJ0 and PJ1 as outputs (vacuum motor IN1, IN2)

    DDRL |= (1 << PL3);  // OC5A (Pin 46) as output

    // Fast PWM, 10-bit: WGM = 0111 (Mode 7)
    TCCR5A |= (1 << WGM50) | (1 << WGM51);
    TCCR5B |= (1 << WGM52);
    TCCR5B &= ~(1 << WGM53);

    // Non-inverting mode on OC5A (COM5A1:0 = 10)
    TCCR5A |= (1 << COM5A1);
    TCCR5A &= ~(1 << COM5A0);

    // Prescaler = 8
    TCCR5B &= ~(1 << CS50);
    TCCR5B |=  (1 << CS51);
    TCCR5B &= ~(1 << CS52);

    // Set duty cycle - 100% is 1023 
    OCR5A = 0;
}

void initMotorVacuumDirections() {
  DDRJ |= (1 << PJ0) | (1 << PJ1);  // Only J0 and J1
}

// Set direction to FORWARD for the vacuum motor
void setVacuumDirectionForward() {
    PORTJ |= (1 << PJ0);   // IN1 = HIGH
    PORTJ &= ~(1 << PJ1);  // IN2 = LOW
}

// Set direction to BACKWARD for the vacuum motor
void setVacuumDirectionBackward() {
    PORTJ &= ~(1 << PJ0);  // IN1 = LOW
    PORTJ |= (1 << PJ1);   // IN2 = HIGH
}

// Stop the vacuum motor (both inputs LOW)
void stopVacuumMotor() {
    PORTJ &= ~(1 << PJ0);
    PORTJ &= ~(1 << PJ1);

    // Optional: also zero the PWM duty cycle
    OCR5A = 0;
}


