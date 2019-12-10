; Lab1.asm
; Created: 2019-11-19
; Created by Johannes Blüml, for the course DA346A at Malmo University.

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		RESET = 0x0000			; reset vector
	.EQU		PM_START = 0x0072		; start of program
	.DEF		TEMP = R16				; Temp registry
	.DEF		COUNTER = R17			; Counter return value from reading keyboard
	.DEF		KEY_FOUND = R18			; Key press detected if bit 0 is 1

;==============================================================================
; Start of program
;==============================================================================
	.CSEG
	.ORG		RESET
	RJMP		init

	.ORG		PM_START
	.INCLUDE	"keyboard.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI			TEMP, LOW(RAMEND)		; Set stack pointer
	OUT			SPL, TEMP				; at the end of RAM.
	LDI			TEMP, HIGH(RAMEND)
	OUT			SPH, TEMP
	RCALL		init_pins				; Initialize pins
	RJMP		main					; Jump to main

;==============================================================================
; Initialize I/O pins
;==============================================================================
init_pins:
	; DDRD are the output pins connected to the leds
	LDI			TEMP, 0x0F
	OUT			DDRD, TEMP

	; Set output pins ; iBridge=>Arduino = P5=>PH4, P6=>PH3, P7=>PE3, P8=>PG5
	; NOTE: Set bit 3 and 4 on DDRH to output ports / have to do it this way since "SBI DDRH,3" does not work
	LDS			TEMP, DDRH
	ORI			TEMP, 0b00011000
	STS			DDRH, TEMP

	SBI			DDRE, 3
	SBI			DDRG, 5

	; Input pins ; iBridge=> Arduino = P17=>PF5, P18=>PF4, P3=>PE4, P4=>PE5
	CBI			DDRF, 5
	CBI			DDRF, 4
	CBI			DDRE, 4
	CBI			DDRE, 5
	
	RET

;==============================================================================
; Main part of program
; Uses registers:
;	R16			Temporary storage from read_keyboard_num
;	R17			Counter from read_keyboard_num
;==============================================================================
main:
    
	CALL		read_keyboard_num			; Read keyboard button that is pressed
	OUT			PORTD, COUNTER				; COUNTER will now contain the pressed key 0-15 so here write it to PORTD which will light up the leds representing a 4bit number
	
	RJMP		main

