/*
 * File:   main.c
 * Author: Valdemar Duk
 * Project: Theremin Instrument
 * Created on November 27, 2018, 1:43 PM
 * 
 * Description: The goal of this project is to create a musical instrument
 * which has both pitch and volume control using distance sensors,
 * and an effect selector using swipe gesture detection.
 * 
 * 
 */


#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "mcc_generated_files/mcc.h"
#include "i2c1.h"
#include "uart.h"
#include "gesture_sensor.h"
#include "libpic30.h"
#include "initint.h"

#define LEFT_SENSOR  1
#define RIGHT_SENSOR 0

int ampl = 0;



int main(void) {
    
    
    int freq = 64;
    
    SYSTEM_Initialize();
    initPWM();
    initT();
    UART2_Initialize(); 
    //outString("TEST");
    I2C1_Initialize(400);
    sensor_Initialize();
    unsigned char rZ, lZ;
    char test_string1[64];
    //outString("TEST");
    _NOSC = 0b001;
    _RCDIV = 0b000;
    
    while(1){
        _T2IE = 0;
        //outString("Hello");
        lZ = (int) (ReadByte(ZPOS,LEFT_SENSOR));
        rZ = (int) (ReadByte(ZPOS,RIGHT_SENSOR));
        __delay32(160000);
        _T2IE
        T2CONbits.TON = 0;
        PR2 = 65000 - (rZ*50);
        T2CONbits.TON = 1;
        ampl = lZ;
        sprintf(test_string1, "Left Range %3d       Right Range: %3d     \r", lZ, rZ);
        outString(test_string1);
        
        
    }
    
    return 0;
}


void _ISR _T2Interrupt(void)
{
    
    static int count = 0;
    //OC1RS = lut[count++];
    OC1RS = 32 + ( ampl* sin(count * 2*3.141592654/64)); //MACRO RESOLUTION WHERE 32 is 50% Duty cycle
    
    if (count++ >= 64) count = 0;                      // reset count at 64
    
    
// clear interrupt flag and exit
    _T2IF = 0;
} // T2 Interrupt
