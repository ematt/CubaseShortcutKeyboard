/*
 * keyboard.h
 *
 * Created: 26-Nov-18 10:53:00 AM
 *  Author: Vlad
 */ 


#ifndef BUTTON_MAP_H_
#define BUTTON_MAP_H_

#include "Button_debounce.h"
#include "encoder.h"

void keyboard_init(void);

void keyboard_routine(void);

uint8_t keyboard_get_pressed_key(void);


#endif /* BUTTON_MAP_H_ */