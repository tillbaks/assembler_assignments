; Lab3.asm
; Created: 2019-12-03
; Created by Johannes Bl�ml, for the course DA346A at Malmo University.

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		RESET = 0x0000			; reset vector
	.EQU		PM_START = 0x0072		; start of program
	.DEF		TEMP = R16				; Temp registry
	.DEF		RVAL = R24				; Counter return value from reading keyboard
	.DEF		KEY = R22				; Currently pressed key

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
	.INCLUDE	"dice.inc"
	.INCLUDE	"stat_data.inc"
	.INCLUDE	"stats.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI			TEMP, LOW(RAMEND)		; Set stack pointer
	OUT			SPL, TEMP				; at the end of RAM.
	LDI			TEMP, HIGH(RAMEND)
	OUT			SPH, TEMP
	RCALL		init_pins				; Initialize pins
	RCALL		lcd_init				; Initialize LCD in lcd.inc
	RCALL		init_stat				; Initialize Stats in stats.inc
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
; Writes welcome to the LCD
; Uses registers:
;	ZH:ZL		Pointer to current character in STR_1
;	R24			Character as Input to lcd_write_char 
;==============================================================================
/*
write_welcome:
	; set D/C pin
	SBI			PORTB, 4						
	; Initialize Z-pointer
	LDI			ZH, HIGH(STR_WELCOME<<1)
	LDI			ZL, LOW(STR_WELCOME<<1)
write_welcome__loop:
	LPM			R24, Z+
	; Check if at end of string
	CPI			R24, 0
	BREQ		write_welcome__done
	; Need to store Z in stack since it is used in lcd_write_char
	PUSH		ZH
	PUSH		ZL
	RCALL		lcd_write_char
	POP			ZL
	POP			ZH
	RJMP		write_welcome__loop
write_welcome__done:
	RET
*/
;==============================================================================
; Main part of program
; Uses registers:
;	R16			Temporary storage from read_keyboard_num
;	R17			Counter from read_keyboard_num
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
	PRINTSTRING	STR_WELCOME

	RCALL		delay_1_s

main_loop:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_PRESS2
main_loop__no_key:
	RCALL		read_keyboard
	CPI			RVAL, NO_KEY
	BREQ		main_loop__no_key
main_loop__check_roll:
	CPI			RVAL, ROLL_KEY
	BRNE		main_loop__check_stat
	RCALL		roll
main_loop__check_stat:
	CPI			RVAL, STAT_KEY
	BRNE		main_loop__check_clear_stat
	RCALL		showstat
main_loop__check_clear_stat:
	CPI			RVAL, CLEAR_STAT_KEY
	BRNE		main_loop__check_monitor
	RCALL		clearstat
main_loop__check_monitor:
	CPI			RVAL, MONITOR_KEY
	BRNE		main_loop
	RCALL		monitor

	RJMP		main_loop





monitor:
	PRINTSTRING	STR_MONITOR
	RCALL		delay_1_s
	RET




















/*


;main_loop:
	RCALL		lcd_clear_display
	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x40 ; row 0
	PRINTSTRING	STR_PRESS2
	RCALL		read_keyboard
	CPI			RVAL, ROLL_KEY
	BRNE		main_loop
	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x40 ; row 0
	PRINTSTRING	STR_ROLLING
	RCALL		roll_dice
	LDI			R24, 48
	ADD			R24, TEMP
	PUSH R24
	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x40 ; row 0
	PRINTSTRING	STR_VALUE
	POP R24
	SBI			PORTB, 4						; set D/C pin
	RCALL		lcd_write_char					; write char to the LCD
	RCALL		store_stat
	RCALL		delay_1_s
	RCALL		main_loop


	; Wait for keypress
	RCALL		read_keyboard_num
	CPI			RVAL, NO_KEY
	BREQ		main_loop

	MOV			KEY, RVAL					; Save RVAL in KEY to compare in key release loop

	; Calculate ascii value from RVAL
	CPI			RVAL, 10
	BRLT		main_loop__less_then_10
	SUBI		RVAL, -7						; Add 7 if RVAL > 9 so 10 becomes ascii A and so on
main_loop__less_then_10:
	SUBI		RVAL, -48

	; Write char to LCD
	SBI			PORTB, 4						; set D/C pin
	MOV			R24, RVAL
	RCALL		lcd_write_char					; write char to the LCD
	
	; Wait for key release
main_loop__wait_release:
	RCALL		read_keyboard_num
	CP			RVAL, KEY
	BREQ		main_loop__wait_release

	; Do it again
	RJMP main_loop
	*/