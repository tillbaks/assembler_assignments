; *****************************************************************************
; stat_data.inc
; Stores an array of values in SRAM which can be increased, cleared and queried
;
; Created: 2019-12-08
; Created by Johannes Bl�ml, for the course DA346A at Malmo University.
; *****************************************************************************

; =============================================================================
; Define constants
; =============================================================================
	.EQU		STAT_DATA_SIZE = 7
; =============================================================================
; Define data
; =============================================================================
	.DSEG
	
	.ORG		0x200
stat_data:
	.BYTE		STAT_DATA_SIZE
	
	.CSEG
; ==============================================================================
; Increases the counter for the dice value in RVAL (index = value in RVAL)
; Also counter for total number of throws (index = 0)
;
; Uses registers:
;	RVAL		Input parameter that contains the dice value also contains total number of this dice value on return
;	TEMP		Temporary counter to increment value at index in stat_data
;	XH:XL		Pointer into stat_data
; =============================================================================
store_stat:
	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	; Increment total throws (Index = 0)
	LD			TEMP, X
	INC			TEMP
	ST			X, TEMP
	; Load dice side to increment (Index = RVAL)
	ADD			XL, RVAL
	CLR			TEMP
	ADC			XH, TEMP
	; Increment dice side
	LD			RVAL, X
	INC			RVAL
	ST			X, RVAL
	RET
; =============================================================================
; Get the stat for a specific dice value / total throws
; The register RVAL will contain the value stored in stat_data on return
; If RVAL = 0, the total number of throws is returned
; otherwise 1-6 it will contain that specific dice value
;
; Uses registers:
;	RVAL		Input and return value
;	XH:XL		Pointer into stat_data
; =============================================================================
get_stat:
	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	ADD			XL, RVAL
	CLR			RVAL
	ADC			XH, RVAL
	LD			RVAL, X
	RET
; =============================================================================
; Clears all stats / reset to 0
;
; Uses registers:
;	TEMP		0's to write to stat_data
;	XH:XL		Pointer into stat_data
; =============================================================================
clear_stat:
	PUSH		COUNTER

	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	LDI			COUNTER, STAT_DATA_SIZE
	CLR			TEMP
; Clear all positions in stat_data
clear_stat__loop:
	ST			X+, TEMP
	DEC			COUNTER
	BRNE		clear_stat__loop

	POP			COUNTER
	RET
