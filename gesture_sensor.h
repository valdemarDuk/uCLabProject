/* 
 * File:   gesture_sensor.h
 * Author: Admin
 *
 * Created on November 27, 2018, 3:42 PM
 */

#ifndef GESTURE_SENSOR_H
#define	GESTURE_SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define STATUS  0x00    //Sensor and Gesture Status 
#define DRE     0x01    //Data Ready Enable Bitmap 
#define DRCFG   0x02    //Data Ready Configuration 
#define GESTURE 0x04    //Last Detected Gesture 
#define GSPEED  0x05    //Last Detected Gesture Speed 
#define DCM     0x06    //Data Confidence Metric 
#define XPOS    0x08    //X Coordinate 
#define ZPOS    0x0a    //Z Coordinate 
#define LRNG    0x0c    //Left Emitter Ranging Data 
#define RRNG    0x0d    //Right Emitter Ranging Data 
#define REGVER  0xfe    //Register Map Version 
#define MODEL   0xff    //Sensor Model ID

    void sensor_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* GESTURE_SENSOR_H */

