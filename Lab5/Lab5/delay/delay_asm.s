
; *****************************************************************************
; delay_asm.s
;
; This file contains delay routines.
;
; Created: 2019-12-13
; Created by Johannes Bluml, for the course DA346A at Malmo University.
; *****************************************************************************

.GLOBAL delay_1_micros
.GLOBAL delay_micros
.GLOBAL delay_ms
.GLOBAL delay_s

; =============================================================================
; Delay of 1 �s (including RCALL)
; =============================================================================
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

; =============================================================================
; Delay of X �s
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R24				Input parameter data (X �s) must be 2 or more for exact delay
; =============================================================================
delay_micros: ; 4 cycle (LDI+RCALL)
	DEC			R24	; 1 cycle

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
	DEC			R24
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

; =============================================================================
; Delay of X ms
;	LDI + RCALL = 4 cycles
;
; 12 extra cycles so less than 1us error
;
; Uses registers:
;	R24				Input parameter data (X ms) and
;					also input to 'delay_micros'.
; =============================================================================
delay_ms: ; 4 cycle
	PUSH		R19
	MOV			R19, R24 ; 1 cycle

delay_ms__wait:
	; Calls delay_micros 4 times with 250 microsecs
	; (250 * 4) us = 1ms
	; then loop amount of times in specified in R24 when called
	LDI			R24, 250
	RCALL		delay_micros
	LDI			R24, 250
	RCALL		delay_micros
	LDI			R24, 250
	RCALL		delay_micros
	LDI			R24, 250
	RCALL		delay_micros

	DEC			R19	; 1 cycle
	BRNE		delay_ms__wait ; 2 cycle

	POP			R19
	RET ; 4 cycle
; =============================================================================
; Delay of X seconds
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R24				Input parameter data (X s) 
; =============================================================================

delay_1_s: ; 4 cycle
	; Calls delay_ms 4 times with 250ms = 1 second
	LDI			R24, 250
	RCALL		delay_ms
	LDI			R24, 250
	RCALL		delay_ms
	LDI			R24, 250
	RCALL		delay_ms
	LDI			R24, 250
	RCALL		delay_ms
	RET

delay_s: ; 4 cycle
	PUSH		R19
	MOV			R19, R24 ; 1 cycle
delay_s__wait:
	PUSH		R19
	RCALL		delay_1_s
	POP			R19

	DEC			R19	; 1 cycle
	BRNE		delay_s__wait ; 2 cycle

	POP			R19
	RET