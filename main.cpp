#include <Arduino.h>
#include <avr/io.h>
#include "spi.h"
#include "pwm.h"
#include "timer.h"
#include <avr/interrupt.h>

unsigned char frowny[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10011001,
  0b10100101,
  0b01000010,
  0b00111100
};

unsigned char smiley[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

typedef enum {
  WaitPress,     // Waiting for press
  DebouncePress,     // Debouncing press
  WaitRelease,     // Waiting for release
  DebounceRelease    // Debouncing release
} STATES1;

typedef enum {
  read_data, 
  buzzerON,
  buzzerOff
} STATES2;


volatile STATES1 state1 = WaitPress;


unsigned char POWER_CTL = 0x2D; // Power Control Register
unsigned char DATA_FORMAT = 0x31; // Data Format Registe





volatile STATES2 place_state = read_data;
volatile STATES1 buttonState = WaitPress;

// Main code of the function
int main(){
  initPWMTimer3();
  initTimer1();
  initTimer0();
  sei();


   


   Serial.begin(9600);
   displayFaceSPI(smiley);


   int i = 1000;
   IncFrequency(i);
   setDutyCycle(0);


   while (1){
       
       MPU.getMPUState();
       Serial.print("X: " + String(MPU.xRot) + "  Y: " + String(MPU.yRot) + "  Z: " + String(MPU.zRot));
       // Serial.println();
      
       switch(buttonState){
           case WaitPress:
             break;
           case DebouncePress:
             delayMs(0.1);
             buttonState = WaitRelease;
             break;
           case WaitRelease:
             break;
           case DebounceRelease:
             delayMs(0.1);
             if(place_state == buzzerON){
               setDutyCycle(0);
                place_state = buzzerOff;
             }


             buttonState = WaitPress;
             break;
         }




       // 
       switch (place_state){
         case read_data:
           Serial.print("Read data");
           if (abs(MPU.yRot) > yRotThresh || abs(MPU.xRot) > xRotThresh){ 
               // Serial.print(" AAHHHH");
               displayFace(frowny);
               place_state = buzzerON;
           }


           else{
               displayFace(smiley);
           }
           delayMs(100);
           break;


         case buzzerON:
           Serial.print("Buzzer state");
           
           for (i = 1000; i < 4000; i++){
               IncFrequency(i);
               setDutyCycle(0.1);
           }
           
           
           
           if (abs(MPU.xRot) > xRotThresh || abs(MPU.yRot) > yRotThresh){ 
               
               displayFace(frowny);
           }


           else{
               displayFace(smiley);
           }


           break;


           case buzzerOff:
           Serial.println("Buzzeroff state");
           //MPU.getMPUState();
           if (abs(MPU.xRot) < xRotThresh && abs(MPU.yRot) < yRotThresh){ 
             // Serial.print(" AAHHHH");
             place_state = read_data;
         }
           


       }
       


       


       Serial.println();


   }


}


ISR(PCINT1_vect){
   
   if(buttonState == WaitPress){
       buttonState = DebouncePress;
       
     }
   
   else if (buttonState == WaitRelease){
     buttonState = DebounceRelease;
     
   }
}
