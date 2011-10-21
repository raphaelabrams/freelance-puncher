
/**
 * Simple Real time clock example by Garth Klee.
 * If run on PICDEM2+, time will be displayed
 * on LCD and will incorporate LED pendulum.
 * Has been written to run on PIC16F87x/A
 */

#include <htc.h>

#define ON 1
#define OFF 0
#define AM 0
#define PM 1

// Set start time here
#define START_H 8
#define START_M 59
#define START_S 45
#define START_AP AM



__CONFIG(XT & WDTDIS & LVPDIS & DEBUGEN);

volatile near unsigned char tickCounter;
volatile near unsigned char newSecond;

near unsigned char hours;
near unsigned char minutes;
near unsigned char seconds;
near bit ampm;

// Pendulum pattern to display on LEDs (RB0:3)
const unsigned char pattern[] = {1,2,4,8,4,2};

void init(void){
	/***** Common Code ****
	 *  Timer 2 interrupt enabled.
	 */
	PIE1	= 0b00000010;
	/*
	 *  Peripheral interrupts enabled
	 *  Global interrupt disabled during initialization
	 */
	INTCON	= 0b01000000;
	/***** Timer 2 Code ****
	 *  Prescale ratio set at 1:16
	 *  Timer2 module activated
	 *  Postscale ratio set at 1:10
	 */
	T2CON	= 0b01001110;
	/*
	 *  Period register set to 0xF9
	 */
	PR2	= 0b11111001;
	ei();	// Global interrupts enabled
}
void main(void){
	init();
	newSecond = tickCounter = 0;
	// Initialise the current time
	hours = START_H;
	minutes = START_M;
	seconds = START_S;

	// Measure time
	while(1){
		if(newSecond){
			// A second has accumulated, count it
			newSecond--;
			if(++seconds > 59){
				seconds=0;
				if(++minutes > 59){
					minutes = 0;
					hours++;
			}
                        }
                }
        }
}

void interrupt isr(void){
		/***** Timer 2 Code *****/
	if((TMR2IE)&&(TMR2IF)){
		// Interrupt period is 40 mSec, 25 interrupts = 1 Sec
		if(++tickCounter == 25){
			tickCounter=0;
			newSecond++;	// Notify a second has accumulated
		}
		TMR2IF=0;	// clear event flag
	}
}
