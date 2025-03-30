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
#include "adc.h"
#include "pwm.h"
#include <avr/interrupt.h>

volatile unsigned char motorState = 1;



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
  initADC();
  initPWMTimer3();
  sei(); // Enable global interrupts.

  unsigned int ADCValue = 0;
  unsigned int dutyCycle = 0;
// while loop
while (1) {
  switch (state) {
    case WaitPress: // Waiting for press 
      delayMS(1);
      motorState = 1;
      changeDutyCycle(0);
      
      
      
      
      
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
      motorState = 0;
      sei(); // Enable interrupts
      state = WaitPress; // Return to waiting state after displaying digits
      break;
  }

  
  if(motorState){
    ADCValue = readADC();

    if(ADCValue < 512){
      motorDirection(0);
      dutyCycle = (511 - ADCValue) * 2;
    }

    else{
      motorDirection(1);
      dutyCycle = (ADCValue - 512) * 2;
    }

    changeDutyCycle(dutyCycle);

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
