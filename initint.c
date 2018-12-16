    
    #include <xc.h>



void initPWM(void){
    OC1RS = 31;                 
    OC1CONbits.OCTSEL = 1;  
    OC1CONbits.OCM = 0x6; 
}

void initT(void){
        //T3 init
        PR3 = 63;            // 6 bit resolution
        T3CON = 0x8000;     //  T3 on
        
        
        //T2 init
        T2CON = 0x8000; //set t2 the same way as T3
        PR2 = 63;
        _T2IE = 1;      // turn on interrupt
        _T2IP = 1;
    }



    
    
