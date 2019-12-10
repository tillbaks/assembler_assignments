/*
 * delay.inc
 *
 * This file contains delay routines.
 *
 * Created: 2019-11-26
 * Created by Johannes Bl�ml, for the course DA346A at Malmo University.
 */

;==============================================================================
; Delay of 1 �s (including RCALL)
;==============================================================================
delay_1_micros:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	RET

;==============================================================================
; Delay of X �s
;	LDI + RCALL = 4 cycles
; Uses registers:
;	RVAL				Input parameter data (X �s) must be 2 or more for exact delay
;==============================================================================
delay_micros: ; 4 cycle (LDI+RCALL)
	DEC			RVAL	; 1 cycle

	; Wait 16 cycle per iteration, 15 cycle last iteration
delay_micros__wait:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	DEC			RVAL
	BRNE		delay_micros__wait

	; NOPS - 8 cycles to fill out remaining microsec
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	RET	; 4 cycle

;==============================================================================
; Delay of X ms
;	LDI + RCALL = 4 cycles
;
; 12 extra cycles so less than 1us error
;
; Uses registers:
;	RVAL				Input parameter data (X ms) and
;					also input to 'delay_micros'.
;==============================================================================
delay_ms: ; 4 cycle
	PUSH		COUNTER
	MOV			COUNTER, RVAL ; 1 cycle

delay_ms__wait:
	; Calls delay_micros 4 times with 250 microsecs
	; (250 * 4) us = 1ms
	; then loop amount of times in specified in RVAL when called
	LDI			RVAL, 250
	RCALL		delay_micros
	LDI			RVAL, 250
	RCALL		delay_micros
	LDI			RVAL, 250
	RCALL		delay_micros
	LDI			RVAL, 250
	RCALL		delay_micros

	DEC			COUNTER	; 1 cycle
	BRNE		delay_ms__wait ; 2 cycle

	POP			COUNTER
	RET ; 4 cycle
;==============================================================================
; Delay of 1 second
;	LDI + RCALL = 4 cycles
; Uses registers:
;	RVAL				Input parameter data (X ms) and also input to 'delay_micros'.
;==============================================================================
delay_1_s: ; 4 cycle
	; Calls delay_ms 4 times with 250ms = 1 second
	LDI			RVAL, 250
	RCALL		delay_ms
	LDI			RVAL, 250
	RCALL		delay_ms
	LDI			RVAL, 250
	RCALL		delay_ms
	LDI			RVAL, 250
	RCALL		delay_ms
	RET