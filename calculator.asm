; This program allows the atmega328p chip to act as a 4-function calculator. My implementation was on an arduino uno board.
; 
.cseg


;make first 2 pins outputs
ldi r16, 0x03
Out ddrb, r16   ;these are pins 8 and 9 on the uno


;make all pins inputs
ldi r16, 0x00
out ddrd, r16	;these are pins 0-7 on the uno

;synchronizer delay
nop

loop:
in r16, pind	;take input from port d

mov r17, r16	;move inputs to register that can be changed
andi r17, 64	;0b01000000 this checks if the "enter" pin (6)
				;is high or low
cpi r17, 64		;compare it to a low value
breq loop		;if it isnt low then keep checking.


button:			;We want the "enter" to work after the button is
				;let go. so the "button" section here will wait
				;for r16 to go low again
in r16, pind
mov r17, r16
andi r17, 64
cpi r17, 0
breq button


operation:		;this will check what operation is being applied
				;and direct branch to the appropriate subroutine.
mov r17, r16	;reset r17 with all inputs

andi r17, 48	;0b00110000 check pins 5 and 4. This will give
				;the users requested operation
				;0b00 -> Addition
				;0b01 -> Subtraction
				;0b10 -> Division
				;0b11 -> Multiplication

cpi r17, 0		;0bxx00xxxx
breq addition
cpi r17, 16		;0bxx01xxxx
breq subtraction
cpi r17, 32		;0bxx10xxxx
breq division
cpi r17, 48		;0bxx11xxxx
breq multiplication

rjmp loop		;if, somehow, none of these options work jump back 
				;to loop. If You run into trouble with your program
				;connect an led to this case that will be set to
				;high when the program passes "breq multiply"
				;without branching



addition:
mov r17, r16	;reapply input to r17
andi r17, 15	;mask 0b00001111 we want the first 4 pins because
				;these are our numbers to be operated on
mov r18, r17	;copy all inputs to r18

lsr r18			;shift input pins 2 and 3 down to orders of 
				;magnitude
lsr r18
andi r17, 3		;bit mask out pins 2 and 3

add r18, r17	;add those bad boys

out portb, r18	;output sum to portb leds 9 and 8

rjmp loop


subtraction:
mov r17, r16	;reapply input to r17
andi r17, 15	;mask 0b00001111 we want the first 4 pins because
				;these are our numbers to be operated on
mov r18, r17	;copy all inputs to r18

lsr r18			;shift input pins 2 and 3 down to orders of 
				;magnitude
lsr r18
andi r17, 3		;bit mask out pins 2 and 3

sub r18, r17	;sub those bad boys

out portb, r18	;output subtraction to portb leds 9 and 8
rjmp loop


division:
ldi r19,0		;r19 will serve as the quotient
mov r17, r16	;reapply input to r17
andi r17,15		;mask 0b00001111 we want the first 4 pins because
				;these are our numbers to be operated on
mov r18, r17	;copy all inputs to r18

lsr r18			;shift input pins 2 and 3 down to orders of 
lsr r18			;magnitude

andi r17, 3		;bit mask out pins 2 and 3
L1:
inc r19			;increment the quotient up one everytime a
				;subtraction is formed
sub r18,r17
brcc L1			;if no carry subtract again

dec r19			;decrement the extra 1 count from the quotient
add r18, r17	;this is the remainder

out portb, r19
rjmp loop


multiplication:

mov r17, r16	;reapply input to r17
andi r17, 15	;mask 0b00001111 we want the first 4 pins because
				;these are our numbers to be operated on
mov r18, r17	;copy all inputs to r18

lsr r18			;shift input pins 2 and 3 down to orders of 
				;magnitude
lsr r18
andi r17, 3		;bit mask out pins 2 and 3

mul r18, r17	;multiply those bad boys
movw r18,r0
out portb, r18	;output subtraction to portb leds 9 and 8

rjmp loop
