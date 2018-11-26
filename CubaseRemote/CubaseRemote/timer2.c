/*
 * timer2.c
 *
 * Created: 24-Nov-18 5:08:20 PM
 *  Author: Vlad
 */ 

#include <avr/interrupt.h>

#include "timer2.h"

#include "Button_debounce.h"


void timer2_init() {
	TCCR2 = ( 1 << CS20 ) | ( 1 << CS21 ) | ( 1 << CS22 );// prescaler 64
	TCCR2 |= ( 1 << WGM21 ); //CTC mode
	OCR2 = 0x9B;// 10 ms at 16Mhz
	
	TIMSK = ( 1 << OCIE2 ); // enable interrupt
}

ISR(TIMER2_COMP_vect) {
	
	button_routine();
	
}
