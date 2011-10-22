
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
#define START_Q 0
#define START_M 0
#define START_S 0

void init(void);
void display(char left, char right, char pile);

//call the following twice (with different args each time) and the compiler automatically sets each config register
__CONFIG(FOSC_LP & WDTE_OFF & PWRTE_ON & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & FCMEN_OFF);
__CONFIG(PLLEN_OFF & WRT_OFF & STVREN_ON & BORV_LO & LVP_OFF );

volatile near unsigned char tickCounter;
volatile near unsigned char newSecond;

near unsigned char centons; //hee hee 100 hours!
near unsigned char hours;
near unsigned char minutes;
near unsigned char seconds;
near unsigned char quarters;
near unsigned char leftbuffer;
near unsigned char rightbuffer;
near unsigned char pilebuffer;
near unsigned char displayenable=5;

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


void display(char left, char right, char pile){
            LATB = ~led[left];
            LATA = common[2];
            LATA = 0;

            LATB = ~led[right];
            LATA = common[1];
            LATA = 0;

            LATB = pile;
            LATA = common[0];
            LATA = 0;
}

void main(void){
    init();
    seconds=eeprom_read(0);
    minutes=eeprom_read(1);
    quarters=eeprom_read(2);
    hours=eeprom_read(3);
    centons=eeprom_read(4);

    // Initialise the current time


    // Measure time


    while(1){

        if(displayenable){
            LATB = leftbuffer;  //left
            LATA = 4;
            LATA = 4;
            LATA = 0;

            LATB = rightbuffer;   //right
            LATA = 2;
            LATA = 2;
            LATA = 0;

            LATB = pilebuffer;     //pile
            LATA = 1;
            LATA = 1;
            LATA = 0;
        }
            //        display(seconds/10,seconds%10,~seconds);
        if(RA4==0){
            PIE1	= 0b00000000;

            eeprom_write(0, seconds);
            eeprom_write(1, minutes);
            eeprom_write(2, quarters);
            eeprom_write(3, hours);
            eeprom_write(4, centons);
            LATA=0b00000110;
            LATB=0b11111101;
            while(RA4==0){;}
        }
    }
}

void interrupt isr(void){
		/***** Timer 2 Code *****/
//	if((TMR2IE)&&(TMR2IF)){
        seconds++;
        if(RA3==0){
            centons=START_C;
            hours = START_H;
            minutes = START_M;
            seconds = START_S;
            LATA=0b00000110;
            LATB=0b11111101;
        }
            if(seconds > 59){
                seconds=0;
                displayenable=1;
                quarters=0;
                if(minutes>=15){quarters=4;}
                if(minutes>=30){quarters=6;}
                if(minutes>=45){quarters=7;}
                minutes++;
                if(++minutes > 59){
                    minutes = 0;
                    hours++;
                    if(hours>99){
                        centons++;
                        hours=0;
                    }
                }
            }

        leftbuffer= ~led[hours/10];
        rightbuffer= ~led[hours%10];
        pilebuffer= ~(((centons*2)&0b00011111)|(quarters*32));
        if(displayenable){displayenable--;}
        else{
            LATA=0b00000110;
            LATB=0b11111110;
            LATA=0b00000000;
        }
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
	T2CON	= 0b01111111;
	/*
	 *  Period register set to 0xF9
	 */
	PR2	= 7;  //TODO check is this precisely right?

        TRISB=0;
        TRISA=0b11111000;
        LATA=0b00000110;
        LATB=0b11111101;
}
