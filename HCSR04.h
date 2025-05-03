#ifndef HCSR04_H 
#define HCSR04_H

#include <Arduino.h>

// Initializes the HC-SR04 sensor 
void HCSR04_init();

// Measures the distance using the HC-SR04 sensor
float HCSR04_distance();

#endif 