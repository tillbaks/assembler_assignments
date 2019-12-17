/*
 * regulator.c
 *
 * This is the device driver for the manual motor speed regulator.
 *
 * Author:	Mathias Beckius
 * Date:	2014-12-08
 *
 * Modified by Mathias Beckius, 26 June 2015, for the course DA346A at
 * Malmo University.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "regulator.h"
#include "../common.h"

// for storage of ADC value from temperature sensor
static volatile uint8_t adc = 0;

/*
 * Interrupt Service Routine for the ADC.
 * The ISR will execute when a A/D conversion is complete.
 */
ISR(ADC_vect)
{
	// read ADC value
	adc = ADCH; // left adjusted so we only need to read high 8 bits
}

/*
 * Initialize the ADC and ISR.
 * ADC Resolution is 8-bit.
 */
void regulator_init(void)
{
	// init A/D conversion
	ADMUX |= 1 << REFS0;							  // set reference voltage (internal 5V - from AVRCC pin)
	ADMUX |= (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // select Single Ended Input for ADC15
	ADCSRB |= 1 << MUX5;							  // ADC15 needs selection in a second place
	ADMUX |= 1 << ADLAR;							  // left adjustment of ADC value

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
	ADCSRA |= 1 << ADATE;								  // enable Auto Trigger
	ADCSRA |= 1 << ADIE;								  // enable Interrupt
	ADCSRA |= 1 << ADEN;								  // enable ADC

	DIDR2 = 1; // disable digital input (PK7) when using ADC15

	// disable USB controller (to make interrupts possible)
	//USBCON = 0;
	// enable global interrupts
	sei();

	// start conversion
	ADCSRA |= 1<<ADSC;
}

/*
 * Returns the value of the regulator in percent: 0-100%
 */
uint8_t regulator_read(void)
{
	uint8_t percentage;
	//add code here to read out current setting
	percentage = (adc/255.0)*100;
	return percentage;
}