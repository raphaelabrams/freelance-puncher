
/**
 * Simple Real time clock example by Garth Klee.
 * If run on PICDEM2+, time will be displayed
 * on LCD and will incorporate LED pendulum.
 * Has been written to run on PIC16F87x/A
 */

#include <htc.h>
//#include<pic16f1827.h>


void init(void);


//call the following twice (with different args each time) and the compiler automatically sets each config register
__CONFIG(FOSC_LP & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & FCMEN_OFF);
__CONFIG(PLLEN_OFF & WRT_OFF & STVREN_ON & BORV_LO & LVP_OFF );

#define _XTAL_FREQ 32768

near unsigned char centons; //hee hee 100 hours!
near unsigned char hours;
near unsigned char minutes;
near unsigned char seconds;
near unsigned char quarters;
near unsigned char leftbuffer;
near unsigned char rightbuffer;
near unsigned char pilebuffer;
near unsigned char displayenable;

//the array for converting a number into a corresponding pattern on the LED display
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


void main(void){
    init();
    while(1){

        if(displayenable){
            LATB = leftbuffer;  //left
            LATA = 4;
            __delay_ms(2);
            LATA = 0;

            LATB = rightbuffer;   //right
            LATA = 2;
            __delay_ms(2);
            LATA = 0;

            LATB = pilebuffer;     //pile
            LATA = 1;
            LATA = 0;
        }
    }
}

void interrupt isr(void){
/***** Timer 2 Code *****/

    seconds++;
    LATA = 0b00000000;
    if(RA3 == 0){           //reset button has been pushed, set all the memory to 0
        centons = 0;
        hours = 0;
        minutes = 0;
        seconds = 0;
        quarters = 0;
        LATA = 0b00000110;
        LATB = 0b11111101;
    }
    if(RA4 == 0){
        PIE1= 0b00000000;  //stop interrupts so we stay here until power runs out
        eeprom_write(0, seconds);  //save all the memory values to eeprom.
        eeprom_write(1, minutes);
        eeprom_write(2, quarters);
        eeprom_write(3, hours);
        eeprom_write(4, centons);
        LATA= 0b00000110;          //indivate that the eeprom is written to by flashing a pattern on the display
        LATB= 0b11111101;
        while(RA4==0){;}  //wait for power to fully dissipate
    }
    if(seconds > 59){
        minutes++;
        seconds = 0;
        quarters = 0;
        if(minutes >= 15){quarters = 4;}
        if(minutes >= 30){quarters = 6;}
        if(minutes >= 45){quarters = 7;}
        if(minutes > 59){
            minutes = 0;
            hours++;
            if(hours > 99){
                centons++;
                hours = 0;
            }
        }
    }
    leftbuffer = ~led[hours/10];
    rightbuffer = ~led[hours%10];
    pilebuffer = ~(((centons*2)&0b00011111)|(quarters*32));
    if(displayenable){
        displayenable--;
    }else{
        LATA = 0b00000110;
        LATB = 0b11111110;
        LATA = 0b00000000;
    }
    TMR2IF = 0;	// clear event flag
}

void init(void){
    TRISB = 0;
    TRISA = 0b11111000;
    ANSELA = 0;
    ANSELB = 0;
    LATA = 0b00000000;
    LATB = 0b11111111;
    seconds = eeprom_read(0);
    minutes = eeprom_read(1);
    quarters = eeprom_read(2);
    hours = eeprom_read(3);
    centons = eeprom_read(4);
    PIE1	= 0b00000010;
//  Peripheral interrupts enabled
    INTCON = 0b11000000;
//  Timer 2 Code
//  Prescale ratio set at 1:64
//  Timer2 module activated
//  Postscale ratio set at 1:16
 //   T2CON = 0b01111111;
    T2CON = 0b01111111;

    PR2 = 7;  //period register
    displayenable = 7;  //set display to show current timo on startup for 7 seconds
}
