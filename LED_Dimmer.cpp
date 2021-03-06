/*
 * LED_Dimmer.cpp
 *
 * Created: 2/23/2021 7:39:17 PM
 * Author : Matthew
 */ 

#define F_CPU	1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle = 0;

void startConversion();
void initADC();

int main(void)
{
	//OC0a Pin
	DDRD = (1 << PORTD6);
	
	
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);	//Fast PWM
	TIMSK0 |= (1 << TOIE0);									//Set Overflow interrupt
	
	//OCR0A = (dutyCycle/100.0)*255.0;
	
	initADC();
	sei();
	
	//No prescalar and start the timer
	TCCR0B |= (1 << CS00);
	
	while(1)
	{
		//TODO:: Got caught up with interrupts and forgot to remove this delay section.
		_delay_ms(10);
		
		dutyCycle += 10;
		
		if(dutyCycle > 100)
		{
			dutyCycle = 0;
		}
	}
}

//Setup the ADC and start the first conversion
void initADC(){
	ADMUX |= (1<<MUX0) | (1<<MUX2) | (1<<REFS0);		//Pin AD5 with VREF connected to AREF and leave the output register right adjusted.
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);	//Enable adc, interrupt flag, and use a adc clock of clk/128
	DIDR0 |= (1<<ADC5D);
	
	startConversion();
}

void startConversion(){
	ADCSRA |= (1<<ADSC);	//Start conversion
}

//PWM Interrupt
ISR(TIMER0_OVF_vect)
{
	OCR0A = dutyCycle; //Output compare register 0A
}

//ADC INTERRUPT
ISR(ADC_vect){
	dutyCycle = ADC/4;			//This is a ten bit value 1024/4 = 256
	
	//For prototyping purposes:
	//Motor is 3.7 volts and battery is 9 volts
	//3.5/9 -> 100/256 - 100 will be our max control saturation
	int saturation = 100;
	if (dutyCycle >= saturation) dutyCycle = saturation;
	startConversion();
}
