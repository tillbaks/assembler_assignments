; *****************************************************************************
; dice.inc represents a Dice that is rolled while a key is pressed
; and on release returns/displays a value from 1 to 6 
; 
; Created: 12/6/2019 12:45:52 PM
; Created by Johannes Blüml, for the course DA346A at Malmo University.
; *****************************************************************************

; =============================================================================
; Define constants
; =============================================================================
	STR_ROLLING:
	.DB				"ROLLING..", 0
	STR_VALUE:
	.DB				"VALUE: ", 0
; =============================================================================
; Rolls a dice and returns a value from 1 to 6 in RVAL
;
; Uses registers:
;	RVAL		Dice value returned from roll_dice
; =============================================================================
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
    ; COUNTER contains the dice value so it is copied to the return value (RVAL)
    MOV         RVAL, COUNTER
    RET
; =============================================================================
; Roll
; Displays "ROLLING.." while key is pressed
; When key released a value between 1 and 6 is returned in RVAL
;
; Uses registers:
;	RVAL		Dice value returned from roll_dice
; =============================================================================
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
	SUBI		RVAL, -48                 ; Convert number to ascii value
	PUSH        RVAL
	RCALL		lcd_clear_display
	PRINTSTRING	STR_VALUE
	POP         RVAL
	RCALL		lcd_write_char
	; Wait a bit so value can be read by humans
	RCALL		delay_1_s
	RET
