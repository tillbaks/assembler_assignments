/*
 * Lab4.c
 *
 * Created: 12/13/2019 1:22:21 PM
 * Author : Johannes
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h> //rand
#include "lcd/lcd.h"
#include "numkey/numkey.h"
#include "delay/delay.h"
#include "hmi/hmi.h"
#include "guess_nr.h"

// for storage of pressed key
char key;
// for generation of variable string
char str[17];

int main(void)

{

	uint16_t rnd_nr;
	// initialize HMI (LCD and numeric keyboard)
	hmi_init();
	// generate seed for the pseudo-random number generator
	srand(1); //random_seed();
	// show start screen for the game
	output_msg("WELCOME!", "LET'S PLAY...", 3);
	// play game
	while (1)
	{
		// generate a random number
		rnd_nr = (rand() % 100) + 1; //random_get_nr(100) + 1;
		// play a round...
		play_guess_nr(rnd_nr);
	}

	/******************************************************************************
	OVANF�R FINNS HUVUDPROGRAMMET, DET SKA NI INTE MODIFIERA!
	NEDANF�R KAN NI SKRIVA ERA TESTER. GL�M INTE ATT PROGRAMMET M?STE HA EN
	O�NDLIG LOOP I SLUTET!

	N�R DET �R DAGS ATT TESTA HUVUDPROGRAMMET KOMMENTERAR NI UT (ELLER RADERAR)
	ER TESTKOD. GL�M INTE ATT AVKOMMENTERA HUVUDPROGRAMMET
******************************************************************************/
	/*
	hmi_init();
	output_msg("WELCOME!", "LET'S PLAY...", 3);
	uint16_t number;
	char *str_enter_num = "ENTER NUMBER:";
	while (1)
	{
		input_int(str_enter_num, &number);
		lcd_write(CHR, ' ');
		lcd_write(CHR, number + 48);
		delay_s(3);
	}
*/
	while (1)
		;
}