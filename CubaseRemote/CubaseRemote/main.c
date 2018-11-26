/*
 * CubaseRemote.c
 *
 * Created: 24-Nov-18 11:31:36 AM
 * Author : Vlad
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>   /* need for usbdrv.h */

#include "usbconfig.h"
#include "usbdrv.h"
#include "oddebug.h"

#include "timer2.h"
#include "encoder.h"

#include "keyboard.h"

#include "USB/usb_hid_keys.h"
#include "USB/usb_hid_consumer.h"


typedef struct
{
	uint8_t  reportId;                                 // Report ID = 0x01 (1)
	// Collection: CA:ConsumerControl
	uint16_t ConsumerControl;                          // Value = 0 to 572
} inputConsumer_t;
	
static uint8_t idleRate;           /* in 4 ms units */

typedef struct
{
	uint8_t  reportId;                                 // Report ID = 0x02 (2)
	// Collection: CA:Keyboard
	uint8_t  KB_KeyboardKeyboardLeftControl  : 1;       // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardLeftShift	 : 1;         // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardLeftAlt		 : 1;           // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardLeftGui		 : 1;           // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardRightControl : 1;      // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardRightShift	 : 1;        // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardRightAlt	 : 1;          // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
	uint8_t  KB_KeyboardKeyboardRightGui	 : 1;          // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
	uint8_t  Keyboard;                                 // Value = 0 to 101
} inputKeyboard_t;

static inputConsumer_t consumer_Report;
static inputKeyboard_t keyboard_report; // sent to PC

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
	0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
	0x09, 0x01,                    // USAGE (Consumer Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, 0x01,                    //   REPORT_ID (1)
	0x19, 0x00,                    //   USAGE_MINIMUM (Unassigned)
	0x2a, 0x3c, 0x02,              //   USAGE_MAXIMUM (AC Format)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0x3c, 0x02,              //   LOGICAL_MAXIMUM (572)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x81, 0x00,                    //   INPUT (Data,Var,Abs)
	0xc0,                          // END_COLLECTION
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x06,                    // USAGE (Keyboard)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, 0x02,                    //   REPORT_ID (2)
	0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
	0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x08,                    //   REPORT_COUNT (8)
	0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x75, 0x08,                    //   REPORT_SIZE (8)
	0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
	0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
	0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
	0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
	0xc0                           // END_COLLECTION

};


// Now only supports letters 'a' to 'z' and 0 (NULL) to clear buttons
void buildKeyboardReport(uint8_t send_key) {
	
	keyboard_report.reportId = 2;
	keyboard_report.KB_KeyboardKeyboardLeftControl	   = 0 ;
	keyboard_report.KB_KeyboardKeyboardLeftShift	   = 0 ;
	keyboard_report.KB_KeyboardKeyboardLeftAlt		   = 0 ;
	keyboard_report.KB_KeyboardKeyboardLeftGui		   = 0 ;
	keyboard_report.KB_KeyboardKeyboardRightControl	   = 0 ;
	keyboard_report.KB_KeyboardKeyboardRightShift	   = 0 ;
	keyboard_report.KB_KeyboardKeyboardRightAlt		   = 0 ;
	keyboard_report.KB_KeyboardKeyboardRightGui		   = 0 ;
	keyboard_report.Keyboard = send_key;
	
}

void buildConsumerReport(uint8_t key)
{
	consumer_Report.reportId = 1;
	consumer_Report.ConsumerControl = key;
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usbRequest_t    *rq = (void *)data;

	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{    /* class request type */
		if(rq->bRequest == USBRQ_HID_GET_REPORT)
		{  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* we only have one report type, so don't look at wValue */
			DBG1(0x21,rq,8);
			if (rq->wValue.bytes[0] == 1)
			{
				buildConsumerReport(KEY_NONE);
				usbMsgPtr = (usbMsgPtr_t)&consumer_Report;
				return sizeof(consumer_Report);
			}
			
			if(rq->wValue.bytes[0] == 2)
			{
				buildKeyboardReport(KEY_NONE);					   
				usbMsgPtr = (usbMsgPtr_t)&keyboard_report;
				return sizeof(keyboard_report);
			}
		}
		else if(rq->bRequest == USBRQ_HID_GET_IDLE)
		{
			usbMsgPtr = (usbMsgPtr_t)&idleRate;
						
			DBG1(0x22,rq,8);
			return 1;

		}else if(rq->bRequest == USBRQ_HID_SET_IDLE){
			DBG1(0x23,rq,8);
			idleRate = rq->wValue.bytes[1];
			
		}else if(rq->bRequest == USBRQ_HID_GET_PROTOCOL){
			DBG1(0x24,rq,8);
			
		}else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL){
			DBG1(0x25,rq,8);
		}
		
		}else{
		/* no vendor specific requests implemented */
	}
	return 0;
}



int main(void)
{
	odDebugInit();
	usbInit();
	keyboard_init();
	usbDeviceDisconnect();
	{
		uchar i = 0;
		while(--i){
			_delay_ms(1);
		}
	}
	
	usbDeviceConnect();
	
	timer2_init();
	sei();
	
	bool mustCloseConsumer = false;
	//bool mustCloseKeyboard = false;
    while (1) 
    {
		usbPoll();   
		keyboard_routine();
		
		if( usbInterruptIsReady()){
			ENCODER_SPIN_DIRECTION encoderDirection = encoder_get_direction();
			
			switch(encoderDirection)
			{
				case ENCODER_SPIN_DIRECTION_LEFT:
				{
					buildConsumerReport(HID_CONSUMER_VOLUME_UP);
					mustCloseConsumer = true;
					usbSetInterrupt((void *)&consumer_Report, sizeof(consumer_Report));
				}
				break;
				case ENCODER_SPIN_DIRECTION_RIGHT:
				{
					buildConsumerReport(HID_CONSUMER_VOLUME_DOWN);
					mustCloseConsumer = true;
					usbSetInterrupt((void *)&consumer_Report, sizeof(consumer_Report));	
				}
				break;
				case ENCODER_SPIN_DIRECTION_NONE:
					if (mustCloseConsumer)
					{
						buildConsumerReport(KEY_NONE);
						mustCloseConsumer = false;
						usbSetInterrupt((void *)&consumer_Report, sizeof(consumer_Report));
						continue;
					}
				default:
				break;
			}
			
			if(encoderDirection == ENCODER_SPIN_DIRECTION_NONE && !mustCloseConsumer)
			{
				uint8_t key = keyboard_get_pressed_key();
				if(key)
				{
					if(key == HID_CONSUMER_MUTE)
					{						
						buildConsumerReport(HID_CONSUMER_MUTE);
						mustCloseConsumer = true;
						usbSetInterrupt((void *)&consumer_Report, sizeof(consumer_Report));
					}
					else
					{
						buildKeyboardReport(key); // fill keyboard_report with 'x' pressed
						//mustCloseKeyboard = true;
						usbSetInterrupt((void *)&keyboard_report, sizeof(keyboard_report));
					}
				}
				else
				{
					buildKeyboardReport(KEY_NONE); // fill keyboard_report with 'x' pressed
					//mustCloseKeyboard = false;
					usbSetInterrupt((void *)&keyboard_report, sizeof(keyboard_report));
				}
			}
			
		}
    }
}

