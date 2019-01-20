#ifndef __global_h__
#define __global_h__

#include "lcd.h"
#include "util.h"

#include <avr/io.h>
#include "global.h"
#include "util.h"

// currently controlled parameter (highlighted on screen)
// [0] bpm [1, 2] signature [3] volume [4] mode
extern volatile uint8_t cursor;

// '>' for switching parameters, '}' for adjusting value
extern volatile char cursor_symbol;

// if true, adjusting value of highlighted parameter
// if false, controlled parameter switching
extern volatile bool edit_active;

// beats per minute
extern volatile uint16_t bpm;

// time signature
extern volatile uint8_t signature;

// duration of metronome beep
extern volatile uint16_t t_beep;

// duration of silence between beeps
extern volatile uint16_t t_sleep;

// sound volume expressed in percent
extern volatile uint8_t volume;

// counter measuring interval between last two taps in miliseconds
extern volatile uint16_t tap_interval_counter;

extern volatile uint8_t pitch_index;

// flag indicating whether tap tempo mode is currently active
extern volatile bool tap_started;

// flag blocking output to speaker
extern volatile bool sound_locked;

// current metronome mode (0 = SOUND_MODE, 1=VIBRT_MODE)
extern volatile bool mode;

// lcd backlight timer (in miliseconds)
extern volatile uint16_t backlight_time_counter;

// indicates whether lcd backlight is on
extern volatile bool backlight_on;

// content to be displayed on LCD
extern char firstLineBuffer[LCD_SIZE + 1];

extern char secondLineBuffer[LCD_SIZE + 1];

#endif