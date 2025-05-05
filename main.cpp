#include "HCSR04.h"
#include "timer.h" 
#include "pwm.h"
#include <Arduino.h>
#include "spi.h"
#include "vacuum.h"

unsigned char StopLED[8] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

unsigned char ForwardLED[8] = {
  0b00000000,
  0b00100000,
  0b01100000,
  0b11111111,
  0b01100000,
  0b00100000,
  0b00000000,
  0b00000000
};

unsigned char ReverseLED[8] = {
  0b00000000,
  0b00000100,
  0b00000110,
  0b11111111,
  0b00000110,
  0b00000100,
  0b00000000,
  0b00000000
};


typedef enum {
  Forward, 
  Stopped,
  Reverse
} STATES;

volatile STATES state = Forward; 
void displayFace(unsigned char face[8]) {
  for (int i = 0; i < 8; i++) {
      write_execute(0x01 + i, face[i]); //
  }
}

int main(){
  sei();
  Serial.begin(9600);
  HCSR04_init();
  SPI_MASTER_Init();
  initPWMTimer3();
  initPWMTimer4();
  initPWMTimer5();
  initMotorVacuumDirections();
  initMotorDirections();
  write_execute(0x0B, 0x07);   // Display all 8 digits
  write_execute(0x09, 0x00);  // No decode mode (directly control segments)
  write_execute(0x0C, 0x01);    // Shutdown mode off (normal operation)
  write_execute(0x0F, 0x00); // No display test
  write_execute(0x0A, 0x08);   // Set brightness (0-15)
  for (int i = 0; i < 8; i++) { write_execute(0x01 + i, 0x00); } // Clear display
  delayMs(100);
  displayFace(ForwardLED); 
  int stop_delay_counter = 0; // Counter for delay in Stopped state
  const int STOP_DELAY_TARGET = 10; // Target delay in Stopped state 
  const int LOOP_DELAY_MS = 100; // Delay for each loop iteration 
while(1){
  float distance = HCSR04_distance();

  Serial.print("Distance: ");
  if (distance > 0) {
      Serial.print(distance);
      Serial.println(" cm");
  } 

  if (distance > 0) {
      // Condition to return to Forward 
      if ((state == Stopped || state == Reverse) && distance > 15) {
          setDirectionRight();
          delayMs(500);
          state = Forward;
      }
      // Condition to enter Stopped 
      else if (state == Forward && distance < 10) {
          state = Stopped;
          stop_delay_counter = 0; 
      }
  }
  // Actions based on Current State 
  switch(state) {
    case Forward:
      displayFace(ForwardLED);
      initPWMTimer3();
      initPWMTimer4();
      initMotorDirections();
      setDirectionForward();
      setVacuumDirectionForward();
      // Motor Forward Code
      break;

    case Stopped:
      displayFace(StopLED);
      // Motor Stop Code
      stopMotors();
      stopVacuumMotor(); 
      stop_delay_counter++; 
      // Check if delay is complete
      if (stop_delay_counter >= STOP_DELAY_TARGET) {
         state = Reverse; 
      }
      break;

    case Reverse:
      displayFace(ReverseLED);
      initPWMTimer3();
      initPWMTimer4();
      initMotorDirections();
      setVacuumDirectionForward();
      setDirectionBackward();
      // Motor Reverse Code
      break;
  }
  delayMs(LOOP_DELAY_MS); 
} 
} 
