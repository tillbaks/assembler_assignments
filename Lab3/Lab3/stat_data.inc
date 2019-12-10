/*
 * stat_data.inc
 *
 *  Created: 12/8/2019 4:33:33 PM
 *   Author: Johannes
 */ 
;
; STAT DATA
;
	.EQU		STAT_DATA_SIZE = 7

	.DSEG
	.ORG		0x200
stat_data:
	.BYTE		STAT_DATA_SIZE
;
; STAT METHODS
;
	.CSEG
;  R24 contains the dice value
; The function will increment the Total number of throws
; and the number of throws with results equals R24.
store_stat:
	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	; Increment total throws
	LD			TEMP, X
	INC			TEMP
	ST			X, TEMP
	; Load dice side to increment
	ADD			XL, R24
	CLR			TEMP
	ADC			XH, TEMP
	; Increment dice side
	LD			R24, X
	INC			R24
	ST			X, R24
	RET
; R24 gives the value for which the result is retrieved.
; If R24 = 0, the total number of throws shall be returned.
; The result is returned in R24.
get_stat:
	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	ADD			XL, R24
	CLR			R24
	ADC			XH, R24
	LD			R24, X
	RET
; No parameters in nor out
clear_stat:
	LDI			XL, LOW(stat_data)
	LDI			XH, HIGH(stat_data)
	LDI			TEMP, STAT_DATA_SIZE
	PUSH		R20
	CLR			R20
clear_stat__loop:
	ST			X+, R20
	DEC			TEMP
	BRNE		clear_stat__loop

	POP			R20
	RET
