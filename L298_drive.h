/*
 * L298_drive.h
 *
 * Created: 2/26/2021 4:37:00 PM
 *  Author: Matthew
 */ 


#ifndef drive_H_
#define drive_H_

//This class will be used to compactly drive the L298 dual h-bridge motor board with the atmega328p

//Note, there is no Enable pin logic here, because that is being taken care of by the pwm elsewhere. Perhaps I should come back and dump all the pwm code here
// with a "setSpeed" function
class L298_driver{
	private:
	char m_reg;
	int m_IN1, m_IN2;
	
	public:
	
	//This function will set pin numbers IN1 and IN2 in register 'register' to output
	void setHBridgePins(char reg, int IN1, int IN2){
		switch (reg)
		{case 'B':
			m_reg = reg;
			m_IN1 = IN1;
			m_IN2 = IN2;
			DDRB |= (1<<IN1) | (1<<IN2);
			break;
			case 'C':
			m_reg = reg;
			m_IN1 = IN1;
			m_IN2 = IN2;
			DDRC |= (1<<IN1) | (1<<IN2);
			break;
			case 'D':
			m_reg = reg;
			m_IN1 = IN1;
			m_IN2 = IN2;
			DDRD |= (1<<IN1) | (1<<IN2);
			break;
		}
	}
	//This function will set the motor to forward
	void forwards(){
		switch (m_reg){
			case 'B':
				PORTB |= (1<<m_IN1);
				PORTB &= ~(1<<m_IN2);
				break;
			case 'C':
				PORTC |= (1<<m_IN1);
				PORTC &= ~(1<<m_IN2);
				break;
			case 'D':
				PORTD |= (1<<m_IN1);
				PORTD &= ~(1<<m_IN2);
				break;
		}
	}
	//This function will set the motor to backwards
	void backwards(){
		switch (m_reg){
			case 'B':
			PORTB |= (1<<m_IN2);
			PORTB &= ~(1<<m_IN1);
			break;
			case 'C':
			PORTC |= (1<<m_IN2);
			PORTC &= ~(1<<m_IN1);
			break;
			case 'D':
			PORTD |= (1<<m_IN2);
			PORTD &= ~(1<<m_IN1);
			break;
		}
	}
	
	};



#endif /* drive_H_ */