/*
 * counter0PWM.cpp
 *
 * Created: 2/13/2021 3:15:51 PM
 * Author : Matthew
 */ 

#include <avr/io.h>
#include <util/delay.h>

void initPWM();


int main(void)
{
    /* Set output pin */
	initPWM();
	
    while (1) 
    {
		//Decrement the Output Compare register every 10 ms. counting down from 255 to 0 and back. Keep in mind the output is inverted. 255 -> 0% duty cycle
		if (OCR0A ==0) OCR0A =255;
		OCR0A--;
		_delay_ms(10);
    }
}


//Counter 0 is 8 bit and using phase-corrected wpm mode
void initPWM(){
	//PRTIM0 must be written to 0 to use the counter 0; This may be unnecessary.
	
	//Pin PD6 should be output
	DDRD |= (1<<6);
	
	//TCCR0A - Timer/Counter Control Register A
	TCCR0A |= (1<<WGM00); //Phase Correct PWM; TOP=0xFF; Update OCR0 at TOP (will need to set this); TOV Flag set on Bottom;
	TCCR0A |= (1<<COM0A1); //Inverted PWM
	int dutyCycle = (255 - OCR0A)*2 / (255*2); //
	
	//TCCR0B - Timer/Counter Control Register B
	//TCCR0B |= (1<< CS00) | (1<<CS02); //Prescaler of 1024;
	TCCR0B |= (1<< CS00);
	//TIMSK0 - TIMER/Counter Interrupt Mask Register
	
	//TIFR0 - Timer/Counter 0 Interrupt Flag Register	
	
	//TCNT0 - Timer/Counter Register <- This is the CURRENT VALUE OF THE TIMER
	//OCR0A - Output compare register A <- This is the VALUE TO COMPARE WITH TIMER VALUE
	OCR0A = 255; //This will turn the led off
	
}