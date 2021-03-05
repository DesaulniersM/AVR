
; This code will find the first 100 numbers in the fibonacci sequence. After that it will restart from the beginning.

; Replace with your application code
start:

;initialize loop restart register
	
	ldi r16, 1
	ldi r19, 10
;Begin Fibonacci loop

nacci:
	inc r20
	mov r17, r16
	add r16, r18

	mov r18, r17
	cp r20, r19
	brge 27
	jmp nacci

    rjmp start
