/*
 * Lab5.c
 * Main program for a state machine that reads a potentiometer
 *
 * Created by Johannes Blüml 2019-12-17 for the course DA346A at Malmö University.
 */

#include <avr/io.h>
#include <stdio.h>
#include "lcd/lcd.h"
#include "numkey/numkey.h"
#include "delay/delay.h"
#include "hmi/hmi.h"
#include "regulator/regulator.h"

enum MOTOR_STATE
{
	MOTOR_OFF,
	MOTOR_ON_FORWARD,
	MOTOR_RUNNING_FORWARD,
	MOTOR_ON_BACKWARD,
	MOTOR_RUNNING_BACKWARD
};
typedef enum MOTOR_STATE state_t;

int main(void)
{

	// initialize HMI (LCD and numeric keyboard)
	hmi_init();
	// Initialize Regulator
	regulator_init();
	// Clear display
	lcd_clear();
	// Variables
	state_t current_state = MOTOR_OFF;
	state_t next_state = MOTOR_OFF;
	
	uint8_t key, regulator;
	char regulator_str[17];

	// Main program loop
	while (1)
	{
		// Update state
		current_state = next_state;
		// Read values
		key = numkey_read();
		regulator = regulator_read();
		// Create output string for regulator value
		sprintf(regulator_str, "MOTOR SPEED:  %u%%", regulator);
		// Run state machine
		switch (current_state)
		{
		case MOTOR_OFF:
			output_msg("MOTOR OFF", "", 0);

			if (key == '1' && regulator == 0)
				next_state = MOTOR_ON_BACKWARD;
			else if (key == '3' && regulator == 0)
				next_state = MOTOR_ON_FORWARD;
			break;

		case MOTOR_ON_FORWARD:
			output_msg("FORWARD", "", 0);

			if (regulator > 0)
				next_state = MOTOR_RUNNING_FORWARD;
			if (key == '0')
				next_state = MOTOR_OFF;
			break;

		case MOTOR_ON_BACKWARD:
			output_msg("BACKWARD", "", 0);

			if (regulator > 0)
				next_state = MOTOR_RUNNING_BACKWARD;
			if (key == '0')
				next_state = MOTOR_OFF;
			break;

		case MOTOR_RUNNING_FORWARD:
			output_msg("FORWARD", regulator_str, 0);

			if (key == '0')
				next_state = MOTOR_OFF;
			break;

		case MOTOR_RUNNING_BACKWARD:
			output_msg("BACKWARD", regulator_str, 0);

			if (key == '0')
				next_state = MOTOR_OFF;
			break;
		}
	}
}