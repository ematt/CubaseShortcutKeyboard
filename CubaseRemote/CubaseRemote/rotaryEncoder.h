/*
 * rotaryEncoder.h
 *
 * Created: 12-Jan-19 10:12:15 AM
 *  Author: Vlad
 */ 


#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_


// Enable this to emit codes twice per step.
//#define HALF_STEP

// Enable weak pullups
#define ENABLE_PULLUPS

typedef enum _ENCODER_SPIN
{
	ENCODER_SPIN_DIRECTION_NONE = 0x00,
	ENCODER_SPIN_DIRECTION_LEFT	= 0x10,
	ENCODER_SPIN_DIRECTION_RIGHT= 0x20,
} ENCODER_SPIN_DIRECTION;

void rotaryEncoder_init();
void rotaryEncoder_process();
ENCODER_SPIN_DIRECTION rotaryEncoder_get_direction();



#endif /* ROTARYENCODER_H_ */