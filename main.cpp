#include "HCSR04.h"
#include "timer.h" 

int main(){
  Serial.begin(9600);
  HCSR04_init();
  while(1){
  float distance = HCSR04_distance();

  // Print the measured distance to the Serial Monitor
  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } 

   delayMs(250);
  }
}