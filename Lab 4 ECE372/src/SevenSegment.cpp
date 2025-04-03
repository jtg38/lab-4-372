// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/29/2025         
// Assignment: Lab 4
// DesciptionL This file implements the initialization of a seven-segment display
//----------------------------------------------------------------------//
#include "SevenSegment.h"
#include <avr/io.h>
#include<avr/interrupt.h>
#include <Arduino.h>

void initSevenSegment() {
    // Configure all PORTC pins for output
    DDRC = 0xFF;
    // Turn off all segments initially
    PORTC = 0x00;
}

void displayDigit(int digit) {
    // Binary patterns for seven-segment display (0-9)
    static const int segments[11] = {
        0b00111111,  // Segments for 0
        0b00000110,  // Segments for 1
        0b01011011,  // Segments for 2
        0b01001111,  // Segments for 3
        0b01100110,  // Segments for 4
        0b01101101,  // Segments for 5
        0b01111101,  // Segments for 6
        0b00000111,  // Segments for 7
        0b01111111,  // Segments for 8
        0b01101111,  // Segments for 9
        0b00000000   // Segments for blank (off)
    };
    PORTC = segments[digit];
}
