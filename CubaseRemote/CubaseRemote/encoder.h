//Fuses LOW FUSE BYTE: 0xCF, HIGH FUSE BYTE: 0x18, LOCKOPT BYTE: 0x3F. 

#ifndef	encoder_h
#define	encoder_h
#include <avr/io.h>

typedef enum _ENCODER_SPIN
{
	ENCODER_SPIN_DIRECTION_NONE = 0,
	ENCODER_SPIN_DIRECTION_LEFT,
	ENCODER_SPIN_DIRECTION_RIGHT,
} ENCODER_SPIN_DIRECTION;

void encoder_init(void);
void encoder_routine(void);
ENCODER_SPIN_DIRECTION encoder_get_direction(void);

#endif  //encoder_h
