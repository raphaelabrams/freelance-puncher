Ha ha!
I suck at readme files.



The Freelance Puncher! It's a device that keeps time while it's on, and saves to eeprom when turnd off. There are two 7-segment led displays for hours, and a bank of discreet leds. The bank of three on the bottom is for quarter hours, and the bank of four above is for hundreds of hours. I hope you bill your client before you reach 499 hours!

The interface consists of two buttons: One to clear the timer back to zero, and the other to switch power on/off and show the display. 

When powered down, time counting stops completely. Thanks to the large capacitors on the power supply, the PIC has just enough time to write all values to eeprom before power is completely gone. By only writing to eeprom then, the total number of eeprom writes is greatly reduced. 

Your total time logged is only displayed on powerup for about 7 seconds. If you want to see your time, just power down then back up. During normal run time the decimal points on the numerals will blink dimly every second. 


Notes:

Beerware is a little low rent for me. This is Scotchware.
Clients! FFS, freelancers should get paid promptly and with minimal fuss.
The Bjork song "Virus" plays at exactly 60bpm. 
I used a Sherline mill with a .02" end mill bit to cut out a prototype. Filling in all the vias was a serious pain!



