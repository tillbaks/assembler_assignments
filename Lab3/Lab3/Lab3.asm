; Lab3.asm
; Created: 2019-12-03
; Created by Johannes Bl�ml, for the course DA346A at Malmo University.

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		RESET = 0x0000			; reset vector
	.EQU		PM_START = 0x0072		; start of program
	.DEF		TEMP = R16				; Temp registry
	.DEF		COUNTER = R19			; Used as counter in loops
	.DEF		RVAL = R24				; Used for different things, mainly as input and output tp subroutines

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
	.INCLUDE	"monitor.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI			TEMP, LOW(RAMEND)		; Set stack pointer
	OUT			SPL, TEMP				; at the end of RAM.
	LDI			TEMP, HIGH(RAMEND)
	OUT			SPH, TEMP

	RCALL		init_pins				; Initialize pins
	RCALL		init_main				; Initialize main application data
	RCALL		lcd_init				; Initialize LCD in lcd.inc
	RCALL		init_stat				; Initialize Stats in stats.inc
	RCALL		init_monitor			; Initialize Monitor in monitor.inc

	RJMP		main					; Init done - start application

;==============================================================================
; Initialize I/O pins
;==============================================================================
init_pins:
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

init_main:
	STR_WELCOME:
	.DB				"WELCOME HUMAN!", 0, 0
	STR_INSTRUCTIONS:
	.DB				"PRESS:        ", \
					"2 TO ROLL     ", \
					"3 FOR STATS   ", \
					"8 TO CLR STATS", \
					"9 FOR MONITOR ", \
					0, 0
	RET
;==============================================================================
; Main part of program
;
; Uses registers:
;	RVAL		Return value from read_keyboard / Current key pressed (ascii)
;==============================================================================
main:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_WELCOME
	RCALL		delay_1_s


main_loop:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_INSTRUCTIONS

; Wait for keypress to avoid printing instruction to lcd constantly
main_loop__no_key:
	RCALL		read_keyboard
	CPI			RVAL, NO_KEY
	BREQ		main_loop__no_key

; Check if any keys that do something in the application are pressed
; and call the corresponding subroutine for that key
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