/*
 *
 * Created: 4/17/2017 11:43:37 AM
 * Author : Zulkar Nayem
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "RFM69.h"
#include "RFM69registers.h"
#include "uart.h"
#define resetPin 3


#define NETWORKID 33
#define NODEID     4

int main(void)
{
	DDRB  |= 0b00000001;
	PORTB |= 0b00000001;		// s�t resetpin h�j
	_delay_ms(1000);
	PORTB &= ~(0b00000001);	// s�t resetpin lav
	#define myUART UART0
	InitUART(myUART, 9600, 8, 'N');
	// initialize RFM69
	rfm69_init(433, NODEID,NETWORKID);
	setHighPower(0);   // if model number rfm69hw
	setPowerLevel(30); // 0-31; 5dBm to 20 dBm 
	encrypt(NULL);     // if set has to be 16 bytes. example: "1234567890123456"

	
	// initialize 16x2 LCD
	
	  
    while (1) 
    {
		//SendString(myUART, "_");
		if(receiveDone())
		{
			_delay_ms(10);
			if(ACKRequested())
				;//sendACK();
			char stringData[16];
			for(uint8_t i=0;i<16;i++) // max 16 digit can be shown in this case
			{
				stringData[i]=DATA[i];
			}
			SendString(myUART, stringData);
			char rssilevel[16];
			sprintf(rssilevel,"\n\nRssi level is : %d \n",readRSSI(0));
			SendString(myUART,	rssilevel);
			
			
		}
    }
}