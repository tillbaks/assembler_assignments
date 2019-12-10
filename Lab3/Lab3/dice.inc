;
; Dice
; 
; Created: 12/6/2019 12:45:52 PM
; Created by Johannes Blüml, for the course DA346A at Malmo University.
;
	STR_ROLLING:
	.DB				"ROLLING..", 0
	STR_VALUE:
	.DB				"VALUE: ", 0

roll:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_ROLLING
    RCALL       roll_dice
    PUSH        RVAL
    RCALL		store_stat
    POP         RVAL
	SUBI		RVAL, -48
	PUSH        RVAL
	RCALL		lcd_clear_display
	PRINTSTRING	STR_VALUE
	POP         RVAL
	SBI			PORTB, 4						; set D/C pin
	RCALL		lcd_write_char					; write char to the LCD
	
	RCALL		delay_1_s
	RET

roll_dice:
    LDI         COUNTER, 6
roll_dice__next:
    PUSH        COUNTER
    RCALL       read_keyboard
    POP         COUNTER
    CPI         RVAL, ROLL_KEY
    BRNE        roll_dice__done         ; Roll key is released = done

    DEC         COUNTER
    BRNE        roll_dice__next         ; RVAL is >0 = loop
    RJMP        roll_dice               ; RVAL is 0 = restart from 6
roll_dice__done:
    MOV         RVAL, COUNTER
    RET

