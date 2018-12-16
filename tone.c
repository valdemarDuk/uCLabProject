#include <xc.h>
#include <math.h>


void _ISR _T2Interrupt(void)
{
    
    static int count = 0;
    //OC1RS = lut[count++];
    OC1RS = 32 + ( 32* sin(count * 2*3.141592654/64)); //MACRO RESOLUTION WHERE 32 is 50% Duty cycle
    
    if (count++ >= 64) count = 0;                      // reset count at 64
    
    
// clear interrupt flag and exit
    _T2IF = 0;
} // T2 Interrupt
