Ha HAAAhhhh!


The Freelance Puncher! It's a device that keeps time while it's on, and saves to eeprom when turned off. There are two 7-segment led displays for hours, and a bank of discreet leds. The bank of three on the bottom is for quarter hours, and the bank of four above is for hundreds of hours. I hope you bill your client before you reach 499 hours!

The interface consists of two buttons: One to clear the timer back to zero, and the other to switch power on/off and show the display.  The second one is the main interface for the device. 

When powered down, time counting stops completely. Thanks to the large capacitors on the power supply, the PIC has just enough time to write all values to eeprom before power is completely gone. By only writing to eeprom then, the total number of eeprom writes is greatly reduced. 

Your total time logged is only displayed on power up, and periodically every 15 minutes, each for about 7 seconds. If you want to see your time on demand, just power down then back up. During normal run time the decimal points on the numerals will blink dimly every second. 

You can order boards from Sparkfun here: http://www.batchpcb.com/product_info.php?products_id=73628&check=4932970ca69c59213fd819d27158301f


Notes:
Beerware is a little low rent for me. This is Scotchware. (Software is free, but if you happen to see me some time, buy me a neat 12 year.)
Clients! FFS, freelancers should get paid promptly and with minimal fuss. This is good business and good humanity!
The Bjork song "Virus" plays at exactly 60bpm. 
I used a Sherline mill with a .02" end mill bit to cut out a prototype. Filling in all the vias was a serious pain! If you have the time, just have a board house make it.

Thanks for listening!
Raphael Abrams
curlybr.com
nycresistor.com




