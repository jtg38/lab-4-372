// Author: Tallen Monnett        
// Net ID: tmonnett        
// Date: 3/29/2025         
// Assignment: Lab 4
// Description: This program implements a simple state machine that uses 
// an external switch to control the display of digits on a seven-segment
// display. The program initializes the timer, switch, and seven-segment 
// display, and uses interrupts to handle switch presses and releases. 
//----------------------------------------------------------------------//



#include <avr/io.h>
#include "SevenSegment.h"
#include "switch.h"
#include "timer.h"
#include <avr/interrupt.h>


typedef enum {
  WaitPress,     // Waiting for press
  DebouncePress,     // Debouncing press
  WaitRelease,     // Waiting for release
  DebounceRelease,      // Debouncing release
  SevenSegment // Display digit
} STATES;

volatile STATES state = WaitPress;

int main(){
  initTimer1();
  initSwitch();
  initTimer0();
  initSevenSegment(); 
  sei(); // Enable global interrupts.
// while loop
while (1) {
  switch (state) {
    case WaitPress:     // Waiting for press 
      break;

    case DebouncePress:     // Debouncing press 
      delayMs(50);
      state = WaitRelease;
      break;

    case WaitRelease:     // Waiting for release 
      break;

    case DebounceRelease:     // Debouncing release 
      delayMs(50);
      state = SevenSegment;
      break;
    case SevenSegment:
      delayMs(50);
      cli(); // Disable interrupts 
      // Display digit on seven-segment display
      for (int i = 9; i >= 0; i--) {
        displayDigit(i); // Display digit
        delaySec(1); // Wait for 1 second
      }
      sei(); // Enable interrupts
      state = WaitPress; // Return to waiting state after displaying digits
      break;
  }
}
  return 0;
}

ISR(INT0_vect) { // Interrupt service routine for switch press/release
  // Check if the switch is pressed or released
  switch(state) {
    case WaitPress: // Waiting for press 
      state = DebouncePress;
      break;
      
    case DebouncePress: // Debouncing press 
      break;
      
    case WaitRelease: // Waiting for release 
      state = DebounceRelease;
      break;
      
    case DebounceRelease: // Debouncing release 
      break;

    case SevenSegment: // Display digit 
      state = WaitPress; 
      break;
  }
}
