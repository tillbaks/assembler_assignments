/*
 * keyboard.inc
 *
 * This file contains a driver for reading the keypad on the
 * "iBridge keypad shield". The driver is adapted for a shield mounted
 * on a Arduino Mega board (ATmega2560).
 *
 * Key mapping:
 *					-----------------   ------------------
 *					| 1 | 2 | 3 | A |   | 0 | 4 | 8  | 12 |
 *					-----------------   ------------------
 *					| 4 | 5 | 6 | B |   | 1 | 5 | 9  | 13 |
 *					-----------------   ------------------
 *					| 7 | 8 | 9 | C |   | 2 | 6 | 10 | 14 |
 *					-----------------   ------------------
 *					| * | 0 | # | D |   | 3 | 7 | 11 | 15 |
 *					-----------------   ------------------
 *
 * Created: 2019-11-26
 * Created by Johannes Bl�ml, for the course DA346A at Malmo University.
 */

;==============================================================================
; Table of characters, for mapping with keys
;==============================================================================
keys:
	.DB			"147*2580369#ABCD", NO_KEY, 0

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		NO_KEY = 0x10			; no key was pressed!
    .EQU        ROLL_KEY = '2'
	.EQU		STAT_KEY = '3'
	.EQU		CLEAR_STAT_KEY = '8'
	.EQU		MONITOR_KEY = '9'

;==============================================================================
; Read keyboard
; Returns the corresponding ascii character for the pressed key
; If no key is pressed, then '\0' (null) is returned.
;
; Uses registers:
;	RVAL		Key returned from read_keyboard_num and then converted to ascii as return value to caller
;	ZH:ZL		Pointer into keys remapping string
;==============================================================================
read_keyboard:
	RCALL		read_keyboard_num
	LDI			ZH, HIGH(keys <<1)				; Initialize Z pointer
	LDI			ZL, LOW(keys <<1)
	ADD			ZL, RVAL						; Add index
	LDI			RVAL, 0x00
	ADC			ZH, RVAL						; Add 0 to catch Carry, if present
	LPM			RVAL, Z

	RET

;==============================================================================
; Read keyboard
; Returns the corresponding number (0-15) of the key pressed.
; Includes code to handle key bounce.
; If no key is pressed, 16 is returned.
;
; Uses registers:
;	RVAL		Delay and Counter for key that is returned to caller
;	TEMP		Temporary storage
;==============================================================================

read_keyboard_num:

; This has to be done for each column to find the key that is pressed
.MACRO CHECK_FOR_KEYPRESS
	; Delay before being able to read pins
	PUSH		RVAL
	LDI			RVAL, 3
	RCALL		delay_ms
	POP			RVAL
	; READ ROW 0
	SBIC		PINF, 5
	RJMP		END
	INC			RVAL
	; READ ROW 1
	SBIC		PINF, 4
	RJMP		END
	INC			RVAL
	; READ ROW 2
	SBIC		PINE, 4
	RJMP		END
	INC			RVAL
	; READ ROW 3
	SBIC		PINE, 5
	RJMP		END
	INC			RVAL
.ENDMACRO

	; Initialize counter which will be used by the caller to know which button is pressed
	LDI			RVAL, 0

	; Check if a key in "COL 0" is pressed
	LDS			TEMP, PORTH
	ANDI		TEMP, 0b11100111
	STS			PORTH, TEMP
	CBI			PORTE, 3
	SBI			PORTG, 5
	CHECK_FOR_KEYPRESS

	; Check if a key in "COL 1" is pressed
	CBI			PORTG, 5
	SBI			PORTE, 3
	CHECK_FOR_KEYPRESS

	; Check if a key in "COL 2" is pressed
	CBI			PORTE, 3
	LDS			TEMP, PORTH
	ORI			TEMP, 0b00001000
	ANDI		TEMP, 0b11101111
	STS			PORTH, TEMP
	CHECK_FOR_KEYPRESS

	; Check if a key in "COL 3" is pressed
	LDS			TEMP, PORTH
	ORI			TEMP, 0b00010000
	ANDI		TEMP, 0b11110111
	STS			PORTH, TEMP
	CHECK_FOR_KEYPRESS

END:
	RET