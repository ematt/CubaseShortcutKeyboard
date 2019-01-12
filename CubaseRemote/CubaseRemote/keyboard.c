/*
 * keyboard.c
 *
 * Created: 26-Nov-18 10:53:09 AM
 *  Author: Vlad
 */ 

#include "globals.h"

#include "keyboard.h"
#include "USB/usb_hid_keys.h"
#include "USB/usb_hid_consumer.h"

enum KEYBOARD_MAP_MDOE
{
	ON_PRESSED = 1,		
};

struct KEYBOARD_KEY {
	const BUTTON btn;
	const enum KEYBOARD_MAP_MDOE mode;
	const uint8_t hidCode;
	uint8_t lastState;
	bool hasPendingAction;
};

static struct KEYBOARD_KEY keyboardMap[] =
{
	{Button_1,		ON_PRESSED, KEY_NONE,				0xFF, false},
	{Button_2,		ON_PRESSED, KEY_NONE,				0xFF, false},              
	{Button_3,		ON_PRESSED, KEY_KPASTERISK,			0xFF, false},
	{Button_4,		ON_PRESSED, KEY_S,					0xFF, false},
	{Button_5,		ON_PRESSED, KEY_KPSLASH,			0xFF, false},
	{Button_6,		ON_PRESSED, KEY_SPACE,				0xFF, false},
	{Button_ENC,	ON_PRESSED, HID_CONSUMER_MUTE,		0xFF, false}
};

void keyboard_init(void)
{
	button_init();
	//encoder_init();
	rotaryEncoder_init();
}

static void keyboard_process_buttons(void)
{
	for(uint8_t i = 0; i < sizeof(keyboardMap) / sizeof(keyboardMap[0]); i++)
	{
		struct KEYBOARD_KEY *key = &keyboardMap[i];
		uint8_t btn_state = button_is_pressed(key->btn);
		if (key->mode == ON_PRESSED)
		{
			if(key->lastState == false && btn_state == true)
			{
				key->hasPendingAction = true;
			}
		}
		
		key->lastState = btn_state;
	}
}

void keyboard_routine(void)
{
	button_routine();
	//encoder_routine();
	rotaryEncoder_process();
	keyboard_process_buttons();
}

uint8_t keyboard_get_pressed_key(void)
{
	for(uint8_t i = 0; i < sizeof(keyboardMap) / sizeof(keyboardMap[0]); i++)
	{
		struct KEYBOARD_KEY *key = &keyboardMap[i];
		
		if(key->hasPendingAction == true)
		{
			key->hasPendingAction = false;
			return key->hidCode;
		}
	}
	return 0;
}
