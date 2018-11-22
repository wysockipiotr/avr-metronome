#ifndef __global_h__
#define __global_h__

#include "util.h"
#include "lcd.h"
#include <avr/io.h>

extern volatile uint8_t cursor;

extern volatile char cursor_symbol;

extern volatile bool edit_active;

extern volatile uint16_t bpm;

extern volatile uint8_t signature;

extern volatile uint16_t t_beep;

extern volatile uint16_t t_sleep;

extern volatile uint8_t volume;

extern volatile char* mode;

extern volatile uint16_t tap_interval_counter;

extern volatile bool tap_started;

extern volatile bool sound_locked;

extern uint8_t signatures[4];

extern char firstLineBuffer[LCD_SIZE + 1];

extern char secondLineBuffer[LCD_SIZE + 1];

#endif //__global_h__