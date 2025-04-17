// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 4/17/2025         
// Assignment: Lab 5
// Description: 
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include <avr/interrupt.h>

unsigned char POWER_CTL = 0x2D; // Power Control Register
unsigned char DATA_FORMAT = 0x31; // Data Format Register

typedef enum {
  WaitPress,     // Waiting for press
  DebouncePress,     // Debouncing press
  WaitRelease,     // Waiting for release
  DebounceRelease,      // Debouncing release
} Button;

typedef enum {
  WaitSmile,
  Smile,
  WaitFrown,
  Frown,
} Face;

volatile Button ButtonState = WaitPress;
volatile Face FaceState = WaitSmile;
int main(){
  initTimer1();
  initSwitch();
  initPWMTimer3();
  spi_write(DATA_FORMAT, 0x00); // configure 4-wire mode by clearing the SPI bit
  spi_write(POWER_CTL, 0x08); // set to measure mode
  sei(); // Enable global interrupts.

// while loop
while (1) {
  switch (ButtonState) {
    case WaitPress: // Waiting for press 
      break;

    case DebouncePress:     // Debouncing press 
      delayMs(1);
      ButtonState = WaitRelease;
      break;

    case WaitRelease:     // Waiting for release 
      break;

    case DebounceRelease:     // Debouncing release 
      delayMs(1);
      ButtonState = WaitRelease;
      break;
  }
  
}
  return 0;
}

ISR(INT0_vect) { // Interrupt service routine for switch press/release
  // Check if the switch is pressed or released
  switch(ButtonState) {
    case WaitPress: // Waiting for press 
      ButtonState = DebouncePress;
      break;
      
    case DebouncePress: // Debouncing press 
      break;
      
    case WaitRelease: // Waiting for release 
      ButtonState = DebounceRelease;
      break;
      
    case DebounceRelease: // Debouncing release 
      ButtonState = WaitPress;
      break;
  }
}


