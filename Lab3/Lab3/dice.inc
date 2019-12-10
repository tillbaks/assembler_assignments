;
; Dice
; 
; Created: 12/6/2019 12:45:52 PM
; Created by Johannes Blüml, for the course DA346A at Malmo University.
;

roll:
	RCALL		lcd_clear_display
	PRINTSTRING	STR_ROLLING
    RCALL       roll_dice
    PUSH        R24
    RCALL		store_stat
    POP         R24
	SUBI		R24, -48
	PUSH        R24
	RCALL		lcd_clear_display
	PRINTSTRING	STR_VALUE
	POP         R24
	SBI			PORTB, 4						; set D/C pin
	RCALL		lcd_write_char					; write char to the LCD
	
	RCALL		delay_1_s
	RET

roll_dice:
    LDI         R19, 6
roll_dice__next:
    NOP
    NOP
    PUSH        R19
    RCALL       read_keyboard
    POP         R19
    CPI         RVAL, ROLL_KEY
    BRNE        roll_dice__done         ; Roll key is released = done

    DEC         R19
    BRNE        roll_dice__next         ; R24 is >0 = loop
    RJMP        roll_dice               ; R24 is 0 = restart from 6
roll_dice__done:
    MOV         R24, R19
    RET

