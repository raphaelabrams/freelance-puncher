
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

void init(void);
void display(char left, char right, char stack);

//call the following twice (with different args each time) and the compiler automatically sets each config register
__CONFIG(FOSC_LP & WDTE_OFF & PWRTE_ON & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & FCMEN_OFF);
__CONFIG(PLLEN_OFF & WRT_OFF & STVREN_ON & BORV_LO & LVP_OFF );

volatile near unsigned char tickCounter;
volatile near unsigned char newSecond;

near unsigned char centons; //hee hee 100 hours!
near unsigned char hours;
near unsigned char minutes;
near unsigned char seconds;
near unsigned char ticks;

const unsigned char led[] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,

    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110
};
const unsigned char common[] ={1,2,4};


void display(char left, char right, char stack){
            LATB = ~led[left];
            LATA = common[2];
            LATA = 0;

            LATB = ~led[right];
            LATA = common[1];
            LATA = 0;

            LATB = stack;
            LATA = common[0];
            LATA = 0;
}

void main(void){
    init();
    newSecond = tickCounter = 0;
    seconds=eeprom_read(0);
    minutes=eeprom_read(1);
    hours=eeprom_read(2);
    centons=eeprom_read(3);

    // Initialise the current time


    // Measure time


    while(1){

            LATB = ~led[seconds/10];  //left
            LATA = common[2];
            LATA = common[2];
            LATA = common[2];
            LATA = 0;

            LATB = ~led[seconds%10];   //right
            LATA = common[1];
            LATA = common[1];
            LATA = common[1];
            LATA = 0;

            LATB = ~seconds;     //stack
            LATA = common[0];
            LATA = common[0];
            LATA = common[0];
            LATA = 0;
//        display(seconds/10,seconds%10,~seconds);
        if(RA4==0){
            PIE1	= 0b00000000;

            LATA=0b00000110;
            LATB=0b11111110;
            eeprom_write(0, seconds);
            eeprom_write(1, minutes);
            eeprom_write(2, hours);
            eeprom_write(3, centons);
            while(RA4==0){;}
        }
        if(RA3==0){
            centons=START_C;
            hours = START_H;
            minutes = START_M;
            seconds = START_S;
        }

        if(newSecond){
            // A second has accumulated, count it
            newSecond=0;
            if(++seconds > 59){
                seconds=0;
                minutes++;
//                LATA = common[minutes%3];
                if(++minutes > 59){
                    minutes = 0;
                    hours++;
                    eeprom_write(0, seconds);
                    eeprom_write(1, minutes);
                    eeprom_write(2, hours);
                    eeprom_write(3, centons);

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
//	if((TMR2IE)&&(TMR2IF)){
		// Interrupt period is 40 mSec, 25 interrupts = 1 Sec
//                ticks++;
//                if(ticks==32){newSecond++;ticks=0;}	// Notify a second has accumulated
                    newSecond++;
  //              display(seconds%10,seconds%10,seconds);

//	}
        TMR2IF=0;	// clear event flag

}

void init(void){
	/***** Common Code ****
	 *  Timer 2 interrupt enabled.
	 */
	PIE1	= 0b00000010;
        ANSELA=0;
        ANSELB=0;

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

        TRISB=0;
        TRISA=0b11111000;
        LATA=0b00000110;
        LATB=0b11111101;
}
