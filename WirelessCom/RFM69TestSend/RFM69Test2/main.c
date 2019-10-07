/*
 *
 * Created: 4/17/2017 11:43:37 AM
 * Author : Zulkar Nayem
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "uart.h"
#include "RFM69.h"
#include "RFM69registers.h"



#define NETWORKID 33
#define NODEID    3
#define TONODEID  4
#define myUART UART0

int main(void)
{
	// initialize RFM69
	if (0)
	{
		rfm69_init(433,NODEID,NETWORKID);
		setHighPower(0); // if model number rfm69hw
		setPowerLevel(30); // 0-31; 5dBm to 20 dBm
		encrypt(NULL); // if set it has to be 16 bytes. example: "1234567890123456"
	}
	
	InitUART(myUART, 9600, 8, 'N');
	SendString(myUART,"Ready for input\n");
	while (1)
	{
		//if(receiveDone())
		//{
			//char stringData[16];
			//for(uint8_t i=0;i<16;i++) // max 16 digit can be shown in this case
			//{
				//stringData[i]=DATA[i];
			//}
			//SendString(myUART,"Received temp: ");
			//SendString(myUART, stringData);
			//SendString(myUART,"\n");
		//}
		if(CharReady(myUART)){
			char readChar = ReadChar(myUART);
			if(readChar == '+'){
				SendString(myUART,"Increment");
				//send(TONODEID,"+",1,0); // (toNodeId,buffer,bufferSize,requestACK?)
			}
			else if(readChar == '-'){
				SendString(myUART,"Decrement");
				//send(TONODEID,"-",1,0); // (toNodeId,buffer,bufferSize,requestACK?)
			}
			else if (readChar != ""){
				char buff[10] = {0};
				sprintf(buff,"%c", readChar);
				SendString(myUART, buff);
			}
		}
		
		//SendString(myUART, "Sent data");
		//send(TONODEID,"Awesome!",8,0); // (toNodeId,buffer,bufferSize,requestACK?)
		//_delay_ms(2000);
	}
}