#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.h"   
#include "timer.h"      
#include "pwm.h"        
#include "spi.h"        
#include "i2c.h"       

// MPU6050 Constants
#define MPU6050_SLA   0x68 // MPU_6050 address with AD0 grounded
#define PWR_MGMT_1    0x6B // Power Management 1 Register
#define WAKEUP        0x00 // Value to write to wake up MPU6050
#define ACCEL_XOUT_H  0x3B // Accelerometer X-axis High Byte Register
#define ACCEL_XOUT_L  0x3C // Accelerometer X-axis Low Byte Register
#define ACCEL_YOUT_H  0x3D // Accelerometer Y-axis High Byte Register
#define ACCEL_YOUT_L  0x3E // Accelerometer Y-axis Low Byte Register
#define ACCEL_ZOUT_H  0x3F // Accelerometer Z-axis High Byte Register
#define ACCEL_ZOUT_L  0x40 // Accelerometer Z-axis Low Byte Register

// LED Matrix Patterns 
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

// State Machine Types 
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

// State Variables 
volatile STATES1 buttonState = WaitPress;
volatile STATES2 systemState = read_data;

// MAX7219 Constants
#define MAX7219_REG_DIGIT0       0x01
#define MAX7219_REG_DECODEMODE   0x09
#define MAX7219_REG_INTENSITY    0x0A
#define MAX7219_REG_SCANLIMIT    0x0B
#define MAX7219_REG_SHUTDOWN     0x0C
#define MAX7219_REG_DISPLAYTEST  0x0F

void displayFace(unsigned char face[8]) {
    for (int i = 0; i < 8; i++) {
        write_execute(MAX7219_REG_DIGIT0 + i, face[i]); //
    }
}

int main(){
    sei(); 
    Serial.begin(9600);
    Serial.println("System Initializing...");

    initTimer1();      
    initSwitch();      
    initPWMTimer4();   
    SPI_MASTER_Init(); 
    InitI2C();         
    Serial.println("Peripherals Initialized.");

    write_execute(MAX7219_REG_SCANLIMIT, 0x07);   // Display all 8 digits
    write_execute(MAX7219_REG_DECODEMODE, 0x00);  // No decode mode (directly control segments)
    write_execute(MAX7219_REG_SHUTDOWN, 0x01);    // Shutdown mode off (normal operation)
    write_execute(MAX7219_REG_DISPLAYTEST, 0x00); // No display test
    write_execute(MAX7219_REG_INTENSITY, 0x08);   // Set brightness (0-15)
    for (int i = 0; i < 8; i++) { write_execute(MAX7219_REG_DIGIT0 + i, 0x00); } // Clear display
    Serial.println("MAX7219 Initialized.");

    Serial.println("Waking up MPU6050...");
    StartI2C_Trans(MPU6050_SLA); // Start I2C transmission
    Write(PWR_MGMT_1);           // Send Power Management register address
    Write(WAKEUP);               // Send Wake up command (0x00)
    StopI2C_Trans();             // Stop I2C transmission
    Serial.println("MPU6050 Wakeup Sent.");
    delayMs(100); 

    displayFace(smiley);      
    change_frequency(1000);   // Set an initial frequency
    change_duty_cycle(0);     

    int xRotThresh = 11000; 
    int yRotThresh = 11000;

    int acc_x = 0;
    int acc_y = 0;
    int acc_z = 0;
    unsigned char high_byte, low_byte;

    Serial.println("Entering Main Loop...");

    while (1){
        // Read X-axis
        Read_from(MPU6050_SLA, ACCEL_XOUT_H); // Point to X_H register
        high_byte = Read_data();              // Read X_H
        Read_from(MPU6050_SLA, ACCEL_XOUT_L); // Point to X_L register
        low_byte = Read_data();               // Read X_L
        acc_x = (int)((high_byte << 8) | low_byte); // Combine bytes

        // Read Y-axis
        Read_from(MPU6050_SLA, ACCEL_YOUT_H); // Point to Y_H register
        high_byte = Read_data();              // Read Y_H
        Read_from(MPU6050_SLA, ACCEL_YOUT_L); // Point to Y_L register
        low_byte = Read_data();               // Read Y_L
        acc_y = (int)((high_byte << 8) | low_byte); // Combine bytes

        // Read Z-axis
        Read_from(MPU6050_SLA, ACCEL_ZOUT_H); // Point to Z_H register
        high_byte = Read_data();              // Read Z_H
        Read_from(MPU6050_SLA, ACCEL_ZOUT_L); // Point to Z_L register
        low_byte = Read_data();               // Read Z_L
        acc_z = (int)((high_byte << 8) | low_byte); // Combine bytes

        // 2. Print Sensor Data
        Serial.print("X: " + String(acc_x) + "  Y: " + String(acc_y) + "  Z: " + String(acc_z));

        // 3. Button Debounce State Machine
        switch(buttonState){
            case WaitPress: break;
            case DebouncePress:
                delayMs(20); 
                buttonState = WaitRelease;
                break;
            case WaitRelease: break;
            case DebounceRelease:
                delayMs(20); //
                if(systemState == buzzerON){
                    change_duty_cycle(0);
                    systemState = buzzerOff;
                    Serial.print("Alarm Silenced");
                }
                buttonState = WaitPress;
                break;
        }

        // 4. System Logic State Machine
        switch (systemState){
            case read_data:
                Serial.print("State: read_data");
                // Check accelerometer thresholds
                if (abs(acc_y) > yRotThresh || abs(acc_x) > xRotThresh){
                    Serial.print("Threshold Exceeded");
                    displayFace(frowny);
                    systemState = buzzerON;
                } else {
                    displayFace(smiley);
                    change_duty_cycle(0); 
                }
                delayMs(50); 
                break;

            case buzzerON:
                Serial.print("State: buzzerON");
                change_frequency(2000); 
                change_duty_cycle(0.1); 
                delayMs(100);           
                if (systemState != buzzerON) break; 

                change_frequency(3000); 
                delayMs(100);          
                if (systemState != buzzerON) break; 
                // Update face 
                if (abs(acc_y) > yRotThresh || abs(acc_x) > xRotThresh){
                    displayFace(frowny);
                } else {
                    displayFace(frowny); 
                }
                break;
            case buzzerOff:
                Serial.print(" | State: buzzerOff");
                // Check thresholds 
                if (abs(acc_x) < xRotThresh && abs(acc_y) < yRotThresh){
                    Serial.print(" -> Stable, back to Read");
                    systemState = read_data;
                    displayFace(smiley);
                } else {
                    displayFace(frowny); 
                }
                delayMs(50); 
                break;
        }
        Serial.println();
    } 
    return 0;
}
// Interrupt Service Routine for Button Press 
ISR(PCINT1_vect){ 
    if(buttonState == WaitPress){
        buttonState = DebouncePress;
    }
    else if (buttonState == WaitRelease){
        buttonState = DebounceRelease;
    }
}
