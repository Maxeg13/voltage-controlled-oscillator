#include<util/delay.h>
#include<avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


#define PORTS_N 6
#define byte uint8_t
#define BAUD 38400                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*2*16UL)-1)//16UL

int16_t a;
byte i;
byte loader;
byte xf=0;
int8_t inc;
byte x=0;
byte xp;
byte xi=0;
byte sign1=0;

void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);

    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);/*|(1<<ADPS0);*/
    ADCSRA |= (1<<ADSC);
}

int8_t adc_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with ’7′ will always keep the value
    // of ‘ch’ between 0 and 7
    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing

    // start single convertion
    // write ’1′ to ADSC


    // wait for conversion to complete
    // ADSC becomes ’0′ again
    // till then, run loop continuously
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));

    return (((ADCL|(ADCH<<8))>>2)-128);
}
//PORTB|=1;
//_delay_us(1);
void main(void)
{
    sei();
    adc_init();
    byte nb=1;
    DDRB=0xFF;

    for(;;)
    {
	//	loader

	inc=adc_read(0);
	xf+=inc;
	x=(byte)(xf);
	sign1=(inc>0);
	if(sign1)
	{

	    if(xp>x)
	    {
		PORTB=~PORTB;
	    }
	}
	else
	{
	    if(xp<x)
	    {
		PORTB=~PORTB;
	    }
	}
	xp=x;
    }
}
