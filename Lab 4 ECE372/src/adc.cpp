// Author: Abril Torres         
// Date:  March 30, 2025      
// Assignment:     Lab 4
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "adc.h"

void initADC(){

  DDRA &= ~(1 << PA0); // Set ADC0 pin as input

  // 1. ADC MUX register set reference voltage set to AVCC = 5V output. 
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);

   // 2. ADCH &ADCL Setting
  // determine left or right justified (ADLAR = 0 RIGHT JUSTIFIED) which is easier 
  ADMUX &= ~(1 << ADLAR);

  // 3. Choose Analog Pin for I/P
  // Specify ADC input channel and mode
  // Set ADC7 as single-ended input with MUX[5:0] = 0b000111
  // Set to channel 7 
  ADCSRB &= ~(1 << MUX5);  
  ADMUX &= ~(1 << MUX4);
  ADMUX &= ~(1 << MUX3);
  ADMUX &= ~(1 << MUX2); 
  ADMUX &= ~(1 << MUX1);
  ADMUX &= ~(1 << MUX0);

  // 4. enable ADC and 5. enable auto-triggering
  ADCSRA|= (1 << ADEN) | (1 << ADATE); // Enable ADC and auto-triggering


  // 6. set Auto Trigger Source Selection
  // Use free-running mode ADTS[2:0] = 0b000 
  ADCSRB &= ~(1 << ADTS2); 
  ADCSRB &= ~(1 << ADTS1);
  ADCSRB &= ~(1 << ADTS0);

   // 7. ADC Sample Rate/ Frequency - configure prescaler 
  // set the ADC clock frequency.  Use a pre-scaler of 128
  // ADC clock frequency is 16 Mhz divided by pre-scaler = 125KHz
  // Sampling rate is 1/ ((1/125K Hz )*(13 clock cycles)) = 9615 KHz
  ADCSRA |= (1 << ADPS2);  
  ADCSRA |= (1 << ADPS1);
  ADCSRA |= (1 << ADPS0);

  //8.  Disable digital input buffer - ADC0 pin digital input - pin A0 - on board to reduce noise 
  DIDR0 |= (1 << ADC0D);

  // 9. Start the ADC conversion
  // Starts and waits for the concersion to be complete, returns the 10-bit result. 

  ADCSRA |= (1 << ADSC); //Start ADC conversion
}

 uint16_t readADC() {
  uint8_t lowBits = ADCL; // Read low byte
  uint8_t highBits = ADCH; // Read high byte
  uint16_t ADCValue = (highBits << 8) | lowBits; // Combine high and low bytes
  return ADCValue; // Return the 10-bit ADC value
}
