; Lab2.asm
; Created: 2019-11-26
; Created by Johannes Blüml, for the course DA346A at Malmo University.

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		RESET = 0x0000			; reset vector
	.EQU		PM_START = 0x0072		; start of program
	.DEF		TEMP = R16				; Temp registry
	.DEF		COUNTER = R19			; Counter return value from reading keyboard
	.DEF		KEY = R20				; Prevoisly pressed key

;==============================================================================
; Start of program
;==============================================================================
	.CSEG
	.ORG		RESET
	RJMP		init

	.ORG		PM_START
	.INCLUDE	"keyboard.inc"
	.INCLUDE	"delay.inc"
	.INCLUDE	"lcd.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI			TEMP, LOW(RAMEND)		; Set stack pointer
	OUT			SPL, TEMP				; at the end of RAM.
	LDI			TEMP, HIGH(RAMEND)
	OUT			SPH, TEMP
	RCALL		init_pins				; Initialize pins
	RCALL		lcd_init				; Initialize LCD
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
;	TEMP			Temporary storage from read_keyboard_num
;	COUNTER			Counter from read_keyboard_num
;	KEY				Previously pressed key
;==============================================================================
main:	

	; MOMENT 1 TEST-PROGRAMM
	/*
	LDI			TEMP, 0x0F
	OUT			PORTD, TEMP
	RCALL		delay_1_s

	LDI			TEMP, 0x00
	OUT			PORTD, TEMP
	RCALL		delay_1_s
	*/

	RCALL		lcd_clear_display
	
	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x40 ; row 0

	LCD_WRITE_CHR 'K'
	LCD_WRITE_CHR 'E'
	LCD_WRITE_CHR 'Y'
	LCD_WRITE_CHR ':'

	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x41 ; row 1

main_loop:
	; Wait for keypress
	RCALL		read_keyboard_num
	CPI			COUNTER, NO_KEY
	BREQ		main_loop

	MOV			KEY, COUNTER					; Save COUNTER in KEY to compare in key release loop

	; Calculate ascii value from COUNTER
	CPI			COUNTER, 10
	BRLT		main_loop__less_then_10
	SUBI		COUNTER, -7						; Add 7 if COUNTER > 9 so 10 becomes ascii A and so on
main_loop__less_then_10:
	SUBI		COUNTER, -48

	; Write char to LCD
	SBI			PORTB, 4						; set D/C pin
	MOV			R24, COUNTER
	RCALL		lcd_write_char					; write char to the LCD
	
	; Wait for key release
main_loop__wait_release:
	RCALL		read_keyboard_num
	CP			COUNTER, KEY
	BREQ		main_loop__wait_release

	; Do it again
	RJMP		main_loop