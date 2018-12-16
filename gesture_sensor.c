#include <stdio.h>
#include "gesture_sensor.h"
#include "i2c1.h"




void sensor_Initialize(void){
    WriteByte(DRE, 0b00000100, 1);
    WriteByte(DRCFG, 0b10000001, 1);
}