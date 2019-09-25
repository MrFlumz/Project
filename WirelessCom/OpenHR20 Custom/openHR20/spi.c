/*
    Copyright (c) 2007 Stefan Engelke <mbox@stefanengelke.de>

    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation 
    files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, 
    modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.

    $Id$
*/

#include <avr/io.h>
#include "spi.h"
#   define SS_DDR                DDRD
#   define SS_PORT              PORTD
#   define SS_PIN                 PD2


void spi_init()
// Initialize pins for spi communication
{
	DDRD |= 1<< 2;
    SPI_DDR &= ~((1<<MOSI)|(1<<MISO)|(1<<SS)|(1<<SCK));
	PORTD |= (1<<MISO);
    // Define the following pins as output
    SPI_DDR |= ((1<<MOSI)|(1<<SCK)|(1<<SS));
	PORTD |= 1<<2; // SS h�j
	   
    SPCR = ((1<<SPE)|               // SPI Enable
            (0<<SPIE)|              // SPI Interupt Enable
            (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|              // Master/Slave select
            (0<<SPR1)|(0<<SPR0)|    // SPI Clock Rate
            (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

    SPSR = (1<<SPI2X);              // Double Clock Rate
    
    // SCK frequency 4MHz with above parameters. Deails in datasheet
    
}

void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	PORTD &= ~(1<<2); // SS Lav
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while((SPSR & (1<<SPIF))==0);
        datain[i] = SPDR;
    }
	PORTD |= 1<<2; // SS h�j
}

void spi_transmit_sync (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
	PORTD &= ~(1<<2); // SS Lav
	
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while((SPSR & (1<<SPIF))==0);
    }
	PORTD |= 1<<2; // SS h�j
}

uint8_t spi_fast_shift (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
	
    SPDR = data;
    while((SPSR & (1<<SPIF))==0);
    return SPDR;
}
