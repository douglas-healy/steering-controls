#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>
#include <stdint.h>

#ifndef TRUE
#define TRUE		(1 == 1)
#define FALSE		(1 == 0)
#endif

typedef void 				(*clickHandler)(void);
typedef unsigned char		stereo_cmd_t;
typedef uint8_t				pin_t;

typedef enum _pin
{
	ONBOARD_LED 						= (pin_t) 13,
	COMMAND_OUTPUT 						= (pin_t) 8,
	LEFT_RESISTOR_LADDER				= (pin_t) A0,
	RIGHT_RESISTOR_LADDER 				= (pin_t) A1,
} pin;

typedef enum _stereo_cmd
{
	STEREO_POWER	   					= (stereo_cmd_t) 0x0A,
	STEREO_VOL_UP      					= (stereo_cmd_t) 0x04,
	STEREO_VOL_DOWN    					= (stereo_cmd_t) 0x05,
	STEREO_SOURCE      					= (stereo_cmd_t) 0x08,
	STEREO_EQUALIZER   					= (stereo_cmd_t) 0x0D,
	STEREO_MUTE        					= (stereo_cmd_t) 0x0E,
	STEREO_NEXT_TRACK  					= (stereo_cmd_t) 0x12,
	STEREO_PREV_TRACK  					= (stereo_cmd_t) 0x13,
	STEREO_FOLDER_FORW 					= (stereo_cmd_t) 0x14,
	STEREO_FOLDER_BACK 					= (stereo_cmd_t) 0x15,
	STEREO_VOICE_CONTROL				= (stereo_cmd_t) 0x1A,
} stereo_cmd;

typedef enum _click_type
{
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_CLICK,
	INVALID_CLICK,
} click_type;

typedef enum _button_type
{
	BTN_MUTE,
	BTN_VOL_UP,
	BTN_VOL_DOWN,
	BTN_SRC,
	BTN_NEXT_TRACK,
	BTN_PREVIOUS_TRACK,
	BTN_INVALID,
} button_type;

typedef struct _button_click
{
	button_type	btn;
	click_type	click;
} click_event;

#endif
