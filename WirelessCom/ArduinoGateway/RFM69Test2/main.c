/*
 *
 * Created: 4/17/2017 11:43:37 AM
 * Author : Zulkar Nayem
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "RFM69.h"
#include "RFM69registers.h"

#define MAX_SIZE 20
#define myUART UART0

#define NETWORKID 33
#define NODEID 4
#define TONODEID 3

#define rx_buffer_size 128
unsigned char rx_buffer[rx_buffer_size];
unsigned char rx_read_pos=0;
unsigned char str_read_pos=0;
unsigned char available=0;

char serialRead();
unsigned char serialAvailable();
char buttonBuffer[100];


int countChars( char* s, char c )
{
	return *s == '\0'
	? 0
	: countChars( s + 1, c ) + (*s == c);
}

int main(void)
{
	

	
	// initialize ADC
	ADMUX |= (1<<REFS0) | (0b0111); // AVVC reference // ADC7
	ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); 
	
	// init PCINT for feedbackbutton
	PCMSK2 = (1 << PCINT19) | (1<<PCINT20);
	PCICR |= (1 << PCIE2);
	DDRD &= ~(0b00011000);
	
	InitUART(myUART, 57600, 8, 'N');
	//SendString(myUART,"Ready for input\n");
	
	// initialize RFM69
	if (1)
	{
		rfm69_init(433,NODEID,NETWORKID);
		setHighPower(0); // if model number rfm69hw
		setPowerLevel(30); // 0-31; 5dBm to 20 dBm
		encrypt(NULL); // if set it has to be 16 bytes. example: "1234567890123456"
	}
	
	sei();
	char str[MAX_SIZE] = "";
	int i=0;
	while (1)
	{
		if(receiveDone())
		{
			char stringData[16];
			for(uint8_t i=0;i<16;i++) // max 16 digit can be shown in this case
			{
				stringData[i]=DATA[i];
			}
			//SendString(myUART,"Received temp: ");
			SendString(myUART, stringData);
			//SendString(myUART,"\n");
		}
		
			
		// if theres stuff in RX buffer
		while(serialAvailable()) 
		{
			str[i]=serialRead();
			i++;
		}
		str[i]='\0';
		if (i > 0 && i < MAX_SIZE) // if string is not empty or at overflow limit
		{
			if (str[i-1] == '!') // string must contain ! at end
			{
				int c = countChars(str,';');
				if (c != 2) // string must contain 2 ;
				{
					SendString(myUART,"Error: Wrong format <SlaveId;Network;RW,Data>\n");	
				}
				else
				{
					_delay_ms(50); // mby remove
					// splits string into seperate strings using ; seperator
					char delim[] = ";";
					char *ptr = strtok(str, delim);
					char input[4][3]; // 4 potential inputfields with a length of 3
					int u = 0;
					while(ptr != NULL)
					{
						sprintf(input[u++],"%s",ptr);
						ptr = strtok(NULL, delim);
					}
					/*SendInteger(myUART, atoi(input[0]));
					SendString(myUART, "..1..\n");
					SendInteger(myUART, atoi(input[1]));
					SendString(myUART, "..2..\n");*/
					
					if (atoi(input[0]) == NODEID) // IF ADRESSED TO SELF
					{
						// start single conversion
						// write ’1? to ADSC
						ADCSRA |= (1<<ADSC);
						// wait for conversion to complete
						// ADSC becomes ’0? again
						// till then, run loop continuously
						while(ADCSRA & (1<<ADSC));
						// 3.3*ADC/1024 gives us voltage in V 
						// each degree results in a 10mV increase
						// so V in converted to mV for at temp of 24,5 degrees we then send 245
						int t = (3.3*ADC/1024)*(1000);
						char sendstr[5];
						sprintf(sendstr,"%d\n",t);
						//SendString(myUART,sendstr);
						
						char buf[4];
						sprintf(buf,"\n");
						strcat(buttonBuffer,buf);
						SendString(myUART,buttonBuffer);
						memset(buttonBuffer,0,sizeof(buttonBuffer));
					}
					else
						send(atoi(input[0]),input[1],sizeof(input[1]),0); // (toNodeId,buffer,bufferSize,requestACK?
				}
				memset(str, 0, sizeof(str));
				memset(rx_buffer, 0, sizeof(rx_buffer));
				i = 0;
				available = 0;
					
			}
		}
		else if (i>MAX_SIZE) // if overflow, reset RX
		{
			SendString(myUART,"Error: string too long\n");
			memset(rx_buffer, 0, sizeof(rx_buffer));
			memset(str, 0, sizeof str);
			i = 0;
			available = 0;
		}
			
			
			
			
			
				//SendInteger(myUART,i);
				
				
				//_delay_ms(100);
				//_delay_ms(100);
				//SendString(myUART, "\n");
			
				
			
				
			
				
			//if(readChar == '+'){
				//SendString(myUART,"Increment");
				////send(TONODEID,"+",1,0); // (toNodeId,buffer,bufferSize,requestACK?)
			//}
			//else if(readChar == '-'){
				//SendString(myUART,"Decrement");
				////send(TONODEID,"-",1,0); // (toNodeId,buffer,bufferSize,requestACK?)
			//}
			//else if (readChar != ""){
				//char buff[10] = {0};
				//sprintf(buff,"%c", readChar);
				//SendString(myUART, buff);
			//}
			
		
		//SendString(myUART, "Sent data");
		//send(TONODEID,"Awesome!",8,0); // (toNodeId,buffer,bufferSize,requestACK?)
		//_delay_ms(2000);
	}
}

char serialRead()
{
	char c='\0';
	if (available>0)
	{
		if(rx_read_pos>=available)
		{
			c=rx_buffer[rx_read_pos-available];
		}
		else
		{
			c=rx_buffer[rx_buffer_size+rx_read_pos-available];
		}
		available-=1;
	}
	return c;
}

unsigned char serialAvailable()
{
	return available;
}



ISR(USART_RX_vect)
{
	rx_buffer[rx_read_pos]=UDR0;
	rx_read_pos++;
	
	if(rx_read_pos>=rx_buffer_size)
	{
		rx_read_pos=0;
	}
	
	available++;
}

int i1 = 0;
int i2 = 0;
ISR(PCINT2_vect){
	_delay_ms(2);
	if((PIND  & 0b00001000)){
		//SendString(myUART, "3 pressed ");
		//SendInteger(myUART, i1++);
		//SendString(myUART, "\n");
		char buf[5];
		sprintf(buf,"$3;-");
		strcat(buttonBuffer,buf);}
		
	else if((PIND  & 0b00010000)){
		//SendString(myUART, "4 pressed ");
		//SendInteger(myUART, i2++);
		//SendString(myUART, "\n");
		char buf[5];
		sprintf(buf,"$3;+");
		strcat(buttonBuffer,buf);
		}
		//SendString(myUART, "bugg pressed\n");
	PCIFR |= 1<<PCIF2;
}