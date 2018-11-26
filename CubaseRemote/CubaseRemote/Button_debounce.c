/*
 * Button.c
 *
 * Created: 24-Nov-18 11:51:44 AM
 *  Author: Vlad
 */ 

#include <avr/io.h>

#include "Button_debounce.h"

struct BUTTON_CONFIGURATION
{
	volatile uint8_t *io;
	const uint8_t mask;
	volatile uint8_t debounce_counter;
	const uint8_t debounce_threshold;
	volatile bool is_pressed;
	volatile uint8_t last_io_state;
	volatile bool actionTaken;
	const char *name;
};

struct BUTTON_CONFIGURATION inputs[] =
{
	{&PINC, 1 << PINC0, 0, 0x0A, true, 1 << PINC0, true, "BTN1"},
	{&PINC, 1 << PINC1, 0, 0x0A, true, 1 << PINC1, true, "BTN2"},
	{&PINC, 1 << PINC2, 0, 0x0A, true, 1 << PINC2, true, "BTN3"},
	{&PINC, 1 << PINC3, 0, 0x0A, true, 1 << PINC3, true, "BTN4"},
	{&PINC, 1 << PINC4, 0, 0x0A, true, 1 << PINC4, true, "BTN5"},
	{&PINC, 1 << PINC5, 0, 0x0A, true, 1 << PINC5, true, "BTN6"},
	{&PINB, 1 << PINB0, 0, 0x0A, true, 1 << PINB0, true, "BTN_ENCODER"},
};

void button_init(void)
{
	DDRC &= ~(1 << DDRC0 | 1 << DDRC1 | 1 << DDRC2 | 1 << DDRC3 | 1 << DDRC4 | 1 << DDRC5);
	PORTC |= (1 << PORTC0 | 1 << PORTC1 | 1 << PORTC2 | 1 << PORTC3 | 1 << PORTC4 | 1 << PORTC5);
	
	DDRB &= ~(1 << DDRB0);
	PORTB |= (1 << PORTB0);
}

inline static void button_process( struct BUTTON_CONFIGURATION *input)
{
	uint8_t current_io_state = *input->io & input->mask;
	
	if(current_io_state != input->last_io_state)
	{
		input->debounce_counter = 0;
		input->actionTaken = false;
	}
	else if(input->actionTaken == false)
	{
		if(input->debounce_counter >= input->debounce_threshold)
		{
			input->is_pressed = input->is_pressed ? false : true;
			input->actionTaken = true;
		}
		else
		{
			input->debounce_counter++;
		}
	}
	
	input->last_io_state = current_io_state;
}

void button_routine(void)
{

	for (uint8_t i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++)
	{
		struct BUTTON_CONFIGURATION *input = &inputs[i];
		button_process(input);
	}

}

bool button_is_pressed(BUTTON btn)
{
	return inputs[(uint8_t)btn].is_pressed == true ? false : true;
}
