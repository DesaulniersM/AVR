/*
*
*
* This program acts as a voltmeter for analog signals from 0 to 5 volts. It then sends the data out over UARt.
* In my case the data was sent to a laptop over usb and read through PuTTY
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


typedef struct portStruct {
	uint8_t PIN;
	uint8_t DDR;
	uint8_t PORT;

} port_t;

volatile port_t *portb=(port_t*) 0x0023;
volatile port_t *portd=(port_t*) 0x0029;
volatile port_t *portc=(port_t*) 0x0026;


int ADCReceive(){
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC))){};	
	return ADCW;
}

void ADCInit()	{
	//Enable ADC
ADCSRA |= (1<<ADEN);
//prescalar
ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
//Both REFSn pins set to zero. so AREF is the reference for the ADC
}
	
void uartInit(){
	UBRR0H = (MYUBRR>>8);
	UBRR0L = (MYUBRR);
	UCSR0B |= (1<<TXEN0);
	//Set frame
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void uartSend(float n){
	char nums[6];
	dtostrf(n,5,3,nums);
	nums[5]='\n';
	
	for (int i=0;i<sizeof(nums);i++)
	{
	while (!(UCSR0A&(1<<UDRE0)))
	{}
	UDR0 = nums[i];
	UCSR0A |= (1<<UDRE0);
	}
}
	

int main(void)
{
	uint16_t rotary;
	float volts;
	
	
	uartInit();
	ADCInit();
   
    while (1) 
    {
		char output[8];
		
	rotary = ADCReceive();
	ADCSRA |=(1<<ADSC);
	
	//convert the rotary encoder value to volts
	volts= rotary*1.0;
	volts = (volts*5)/1023;

	uartSend(volts);
	}
}