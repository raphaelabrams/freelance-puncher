
/**
 * Simple Real time clock example by Garth Klee.
 * If run on PICDEM2+, time will be displayed
 * on LCD and will incorporate LED pendulum.
 * Has been written to run on PIC16F87x/A
 */

#include <htc.h>
//#include<pic16f1827.h>

#define ON 1
#define OFF 0
#define AM 0
#define PM 1

// Set start time here
#define START_C 0
#define START_H 0
#define START_M 0
#define START_S 55


//call the following twice (with different args each time) and the compiler automatically sets each config register
__CONFIG(FOSC_LP & WDTE_OFF & PWRTE_ON & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & FCMEN_OFF);
__CONFIG(PLLEN_OFF & WRT_OFF & STVREN_ON & BORV_LO & LVP_OFF );

volatile near unsigned char tickCounter;
volatile near unsigned char newSecond;

near unsigned char centons; //hee hee 100 hours!
near unsigned char hours;
near unsigned char minutes;
near unsigned char seconds;

const unsigned char led[] = {1,2,4,8,16,32,64,128};
const unsigned char common[] ={1,2,4};
void init(void){
	/***** Common Code ****
	 *  Timer 2 interrupt enabled.
	 */
	PIE1	= 0b00000010;
	/*
	 *  Peripheral interrupts enabled
	 *  Global interrupt disabled during initialization
	 */
	INTCON	= 0b11000000;
	/***** Timer 2 Code ****
	 *  Prescale ratio set at 1:64
	 *  Timer2 module activated
	 *  Postscale ratio set at 1:16
	 */
	T2CON	= 0b01111101;
	/*
	 *  Period register set to 0xF9
	 */
	PR2	= 127;  //TODO check is this precisely right?
//	ei();	// Global interrupts enabled

        TRISB=0;
        TRISA=0b11111000;
        LATA=0b00000110;
        LATB=0;
}
void main(void){
    init();
    newSecond = tickCounter = 0;
    // Initialise the current time
    if(RA3==1){
        centons=START_C;
        hours = START_H;
        minutes = START_M;
        seconds = START_S;
    }
//    if(RA4==0){
//        LATA=0b00000111;
//        LATB=0;
//        while(RA4==0){;}
//    }
    // Measure time
    while(1){
        if(newSecond){
            LATB = ~led[seconds%8];
            // A second has accumulated, count it
            newSecond=0;
            if(++seconds > 59){
                seconds=0;
                minutes++;
                LATA = common[minutes%3];
                if(++minutes > 59){
                    minutes = 0;
                    hours++;
                    if(hours>99){
                        centons++;
                        hours=0;
                    }
                }
            }
        }
    }
}

void interrupt isr(void){
		/***** Timer 2 Code *****/
	if((TMR2IE)&&(TMR2IF)){
		// Interrupt period is 40 mSec, 25 interrupts = 1 Sec
		newSecond++;	// Notify a second has accumulated
		TMR2IF=0;	// clear event flag
	}
}
