; keyboard.inc
;
; This file contains a driver for reading the keypad on the
; "iBridge keypad shield". The driver is adapted for a shield mounted
; on a Arduino Mega board (ATmega2560).
;
; Key mapping:
;					-----------------   ------------------
;					| 1 | 2 | 3 | A |   | 0 | 4 | 8  | 12 |
;					-----------------   ------------------
;					| 4 | 5 | 6 | B |   | 1 | 5 | 9  | 13 |
;					-----------------   ------------------
;					| 7 | 8 | 9 | C |   | 2 | 6 | 10 | 14 |
;					-----------------   ------------------
;					| * | 0 | # | D |   | 3 | 7 | 11 | 15 |
;					-----------------   ------------------
;
; Created: 2019-11-19
; Created by Johannes Blüml, for the course DA346A at Malmo University.

;==============================================================================
; Table of characters, for mapping with keys
;==============================================================================
keys:
	.DB			"XXXXXXXXXXXXXXXX", NO_KEY, 0

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU		NO_KEY = 0x10			; no key was pressed!

;==============================================================================
; Read keyboard
; Returns the corresponding character for the pressed key. If no key is
; pressed, then '\0' (null) is returned.
;
; Uses registers:
;	R19			Counter and contains key when returning
;
; Important! A minimum delay of 2 NOP's is necessary after setting port,
; before pin can be read.
;==============================================================================
read_keyboard:

; write code here to implement function in Lab 2
; hint: use read_keyboard_num and just convert/map to character here
	RET

;==============================================================================
; Read keyboard
; Returns the corresponding number (0-15) of the key pressed.
; Includes code to handle key bounce.
; If no key is pressed, 16 is returned.
;
; Uses registers:
;	R17 (COUNTER)		Counter from that is returned to callee
;	R16 (TEMP)		Temporary storage
;==============================================================================
read_keyboard_num:
	

	; This has to be done for each column to find the key that is pressed
	.MACRO CHECK_FOR_KEYPRESS
		; Wait 2 cycles before being able to read pins
		NOP
		NOP
		; READ ROW 0
		SBIC		PINF, 5
		RJMP		END
		INC			COUNTER
		; READ ROW 1
		SBIC		PINF, 4
		RJMP		END
		INC			COUNTER
		; READ ROW 2
		SBIC		PINE, 4
		RJMP		END
		INC			COUNTER
		; READ ROW 3
		SBIC		PINE, 5
		RJMP		END
		INC			COUNTER
	.ENDMACRO

	; Initialize counter which will be used by the callee to know which button is pressed
	LDI			COUNTER, 0

	; Check if a key in "COL 0" is pressed
	LDS			TEMP, PORTH
	ANDI		TEMP, 0b11100111
	STS			PORTH, TEMP
	CBI			PORTE,3
	SBI			PORTG,5
	CHECK_FOR_KEYPRESS

	; Check if a key in "COL 1" is pressed
	SBI			PORTE,3
	CBI			PORTG,5
	CHECK_FOR_KEYPRESS

	; Check if a key in "COL 2" is pressed
	CBI			PORTE,3
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



        ; If you reach this point, counter should equal NO_KEY
END:    ; You may come here through a jump statement above
RET     ; return with correct value in counter
