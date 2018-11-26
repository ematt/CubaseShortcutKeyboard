#include "encoder.h"

#define ENCODER_PORT PORTD
#define ENCODER_DDR DDRD
#define ENCODER_PIN PIND

#define ENCODER_PIN1 PIND6
#define ENCODER_PIN2 PIND7


#define b00000011 3
#define b11010010 210
#define b11100001 225

volatile ENCODER_SPIN_DIRECTION encoder_direction;

void encoder_init(void)
{
	ENCODER_DDR  &= ~((1 << ENCODER_PIN1) | (1<<ENCODER_PIN2));
	ENCODER_PORT |=  ((1 << ENCODER_PIN1) | (1<<ENCODER_PIN2));
	
	encoder_direction = ENCODER_SPIN_DIRECTION_NONE;
}

void encoder_routine(void)
{
	static unsigned char stateEnc; 	
	unsigned char tmp;  
	unsigned char currentState = 0;
	
	if ((ENCODER_PIN & (1<<ENCODER_PIN1))!= 0) 
	{
		currentState |= (1 << 0);
	}
	if ((ENCODER_PIN & (1<<ENCODER_PIN2))!= 0) 
	{
		currentState |= (1 << 1);
	}

	tmp = stateEnc;
	if (currentState == (tmp & b00000011)) 
		return;

	tmp = (tmp<<2)|currentState;
	stateEnc = tmp;

	encoder_direction = ENCODER_SPIN_DIRECTION_NONE;

	if (tmp == b11100001) 
		encoder_direction = ENCODER_SPIN_DIRECTION_LEFT;
	if (tmp == b11010010) 
		encoder_direction = ENCODER_SPIN_DIRECTION_RIGHT;
}


ENCODER_SPIN_DIRECTION encoder_get_direction(void)
{
	return encoder_direction;
}

