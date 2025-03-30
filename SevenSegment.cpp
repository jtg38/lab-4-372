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
    DDRC = 0xFF; // Set PORTC as output
    PORTC = 0x00; // Initialize all segments off 
}

void displayDigit(int digit) {
    // Lookup table for digits 0-9 on a common cathode seven-segment display
    static const int lookupTable[10] = {
        0b00111111,  // 0
        0b00000110,  // 1
        0b01011011,  // 2
        0b01001111,  // 3
        0b01100110,  // 4
        0b01101101,  // 5
        0b01111101,  // 6
        0b00000111,  // 7
        0b01111111,  // 8
        0b01101111,  // 9
    };
    PORTC = lookupTable[digit]; // Display the digit
}