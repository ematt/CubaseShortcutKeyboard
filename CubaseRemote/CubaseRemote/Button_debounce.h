/*
 * Button.h
 *
 * Created: 24-Nov-18 11:51:50 AM
 *  Author: Vlad
 */ 


#ifndef BUTTON_DEBOUNCE_H_
#define BUTTON_DEBOUNCE_H_

#include "globals.h"

typedef enum Button {
	Button_1	= 0, 
	Button_2	= 1, 
	Button_3	= 2, 
	Button_4	= 3,
	Button_5	= 4,
	Button_6	= 5,
	Button_ENC	= 6
} BUTTON;

void button_init(void);

void button_routine(void);

bool button_is_pressed(BUTTON btn);





#endif /* BUTTON_DEBOUNCE_H_ */