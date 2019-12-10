;
; Dice
; 
; Created: 12/6/2019 12:45:52 PM
; Created by Johannes Blüml, for the course DA346A at Malmo University.
;

;==============================================================================
; Define constants
;==============================================================================
	STR_ROLLING:
	.DB				"ROLLING..", 0
	STR_VALUE:
	.DB				"VALUE: ", 0
;==============================================================================
; Roll
; Displays "ROLLING.." while key is pressed
; When key released a value between 1 and 6 is returned in RVAL
;
; Uses registers:
;	RVAL		Dice value returned from roll_dice
;==============================================================================
roll:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_ROLLING
    ; Roll the dice
    RCALL       roll_dice
    ; Store rolled dice value to stats
    PUSH        RVAL
    RCALL		store_stat
    POP         RVAL
    ; Write dice value to display
	SUBI		RVAL, -48 ; Convert number to ascii value
	PUSH        RVAL
	RCALL		lcd_clear_display
	PRINTSTRING	STR_VALUE
	POP         RVAL
	SBI			PORTB, 4 ; set D/C pin
	RCALL		lcd_write_char
	; Wait a bit so value can be read by humans
	RCALL		delay_1_s
	RET
;==============================================================================
; Rolls a dice and returns a value from 1 to 6 in RVAL
;
; Uses registers:
;	RVAL		Dice value returned from roll_dice
;   COUNTER     Used to count from 6 to 1
;==============================================================================
roll_dice:
    ; Start from 6
    LDI         COUNTER, 6
    ; And count down while ROLL_KEY is pressed down
roll_dice__next:
    RCALL       read_keyboard
    CPI         RVAL, ROLL_KEY
    BRNE        roll_dice__done         ; Roll key is released = done

    DEC         COUNTER
    BRNE        roll_dice__next         ; RVAL is >0 = loop
    RJMP        roll_dice               ; RVAL is 0 = restart from 6
roll_dice__done:
    MOV         RVAL, COUNTER
    RET

