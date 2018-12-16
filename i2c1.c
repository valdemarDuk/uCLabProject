/**
 * Filename	: I2C1.C 
 * 
 * Author	: Day Yann Fong (2018)
 * 
 * Descriptions:
 * These source code are created based on Explorer16 board PIC24 platform, using 
 * I2C1. It contains functions that perform basic general I2C operations and 
 * transactions. 
 * No device specific code should be added into this generic functions.
 * 
**/

#include "i2c1.h"
#include "uart2.h"

#define FCY 16000000   // Fcy = 16MHz
#define addr1 (0x20) // I²C address of SENSOR shifted by 1 bit
#define addr2 (0x22) // I²C address of SENSOR shifted by 1 bit

/*
    Operation: Perform an I2C initialization.
	<uiSpeed_Khz> is parameter passed by caller, indicates the desired speed of I2C
	comunication

*/
void I2C1_Initialize(unsigned int uiSpeed_Khz) 
{
     
	// **enter your code for I2C1BRG**
    // Initialiaze the I2C1 Peripherial for Master Mode,
                 // Baud Rate Generator Value
    int brg = 37;//(FCY/uiSpeed_Khz*1000L)-(FCY/10000000L)-1;
    I2C1CON = 0x0200;
    I2C1BRG = brg;
    // Clear both receive and transmit buffers	
    I2C1RCV = 0x0000; 		
	I2C1TRN = 0x0000; 		
    
    // Enables the I2Cx module and configures the SDAx and SCLx pins as serial port pins
    I2C1CONbits.I2CEN = 1;	
}

/*
    Operation: Perform an I2C start operation.
 * 
*/
void startI2C1(void) 
{
    // initiate start on I2C
    I2C1CONbits.SEN = 1; 
    // wait until start finished
    while (I2C1CONbits.SEN);
}

/*
    Operation: Perform an I2C stop operation.
*/
void stopI2C1(void) 
{
    // initiate stop condition on I2C
    I2C1CONbits.PEN=1;   
    
    // wait until stop finished
    while (I2C1CONbits.PEN);  
}

/*
    Operation: Send one byte <cSendByte>, if NAK is returned use outString() 
	function to display the error via UART interface.
*/
void putI2C1(char cSendByte) 
{
    I2C1TRN = cSendByte; 
  
    // wait for 8bits+ack bit from slave to finish
    while (I2C1STATbits.TRSTAT);
    
    if (I2C1STATbits.ACKSTAT != I2C_ACK) 
    {
        //NAK returned
        outString("*** I2C1PUT, NAK returned ***");
    }
}

/*
    Operation: Wait for a byte on the I2C bus, send <cAck2Send> as the 
	acknowledgement bit to be sent to the slave.
    The byte read will be passed as return value of the function.
*/
char getI2C1(char cAck2Send) 
{
    char cRcvByte;

    //wait for idle condition, lower 5 bits must be 0
    while (I2C1CON & 0x1F);   
    
    // enable receiving
    I2C1CONbits.RCEN = 1;    
	while (!I2C1STATbits.RBF);		//wait for receive to complete     

	//read byte;
    cRcvByte = I2C1RCV;           
    
    //wait for idle condition before attempting ACK
    while (I2C1CON & 0x1F);         
    
    I2C1CONbits.ACKDT = cAck2Send; 
    I2C1CONbits.ACKEN = 1;          //enable ACKbit transmittion
    while (I2C1CONbits.ACKEN);      //wait for completion
    
    return(cRcvByte);                  
}

/*
    Transaction: Write 1 byte <ucData> to I2C slave at address <ucAddr>
*/
void write1I2C1(char cAddr, char cSendByte) 
{
    startI2C1();
    putI2C1(I2C_WADDR(cAddr));
    putI2C1(cSendByte);
    stopI2C1();
}

/*
    Transaction: Write <iCnt> number of bytes stored in buffer <pcArrayData> 
	to I2C slave at address <cAddr>.
*/
void writeNI2C1(char cAddr, char* pcArraydata, int iCnt) 
{
    startI2C1();
    putI2C1(I2C_WADDR(cAddr));
     
    int x;
    for(x = 0; x < iCnt; x++){
        putI2C1(*(pcArraydata+x));
    }
    
    stopI2C1();
}

/*
    Transaction: Read one byte from I2C slave at address <cAddr>, save to 
	pointer to location <pcRcvByte>.
	As per the I2C standard, a NAK is returned for the last byte read from the 
	slave.
*/
void read1I2C1(char cAddr, char* pcRcvByte) 
{
    startI2C1();
    putI2C1(I2C_RADDR(cAddr));
    
    //last ack bit from master to slave during read must be a NAK
    *pcRcvByte = getI2C1(I2C_NAK); 
    stopI2C1();
}


/*
    Transaction: Read <iCnt> number of bytes from I2C slave at address <cAddr>, 
    save to buffer pointer <*pcArrayData>
    As per the I2C standard, a NAK is returned for the last byte read from the 
	slave, while ACKs are returned for the other bytes.
*/
void readNI2C1(char cAddr, char* pcArrayData, int iCnt) 
{
    startI2C1();
    putI2C1(I2C_RADDR(cAddr));
    int i;
    for (i = 0; i < iCnt-1; i++){
        *(pcArrayData+i) = getI2C1(I2C_ACK);
    }
    i++;
    *(pcArrayData+i) = getI2C1(I2C_NAK);
}


///////////////////////////////////////////////////////////////////
// Split 16-bit register address into two bytes and write
// the address + data via I²C
///////////////////////////////////////////////////////////////////
void WriteByte(unsigned char reg,char data, int selectvar1) {
 char data_write[2];
 //data_write[0] = (reg >> 8) & 0xFF;; // MSB of register address
 data_write[0] = reg & 0xFF; // LSB of register address
 data_write[1] = data & 0xFF;
 if(selectvar1){
 writeNI2C1(addr1, data_write, 2);
}
 else writeNI2C1(addr2, data_write, 2);
}
///////////////////////////////////////////////////////////////////
// Split 16-bit register address into two bytes and write
// required register address to VL6180X and read the data back
///////////////////////////////////////////////////////////////////
char ReadByte (unsigned char reg, int selectvar) {
 char data_write[1];
 char data_read[1];
 data_write[0] = reg & 0xFF; // LSB of register address
 if(selectvar){
    writeNI2C1(addr1, data_write, 1);
    read1I2C1(addr1, data_read);
 }
 else{
    writeNI2C1(addr2, data_write, 1);
    read1I2C1(addr2, data_read);
 }
 return data_read[0];
}


