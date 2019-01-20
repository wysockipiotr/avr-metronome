#include "global.h"
#include <avr/io.h>
#include "setup.h"
#include "util.h"

// currently controlled parameter (highlighted on screen)
// [0] bpm [1, 2] signature [3] volume [4] mode
volatile uint8_t cursor;

// '>' for switching parameters, '}' for adjusting value
volatile char cursor_symbol;

// if true, adjusting value of highlighted parameter
// if false, controlled parameter switching
volatile bool edit_active;

// beats per minute
volatile uint16_t bpm;

// time signature
volatile uint8_t signature;

// duration of metronome beep
volatile uint16_t t_beep;

// duration of silence between beeps
volatile uint16_t t_sleep;

// sound volume expressed in percent
volatile uint8_t volume;

// counter measuring interval between last two taps in miliseconds
volatile uint16_t tap_interval_counter;

// flag indicating whether tap tempo mode is currently active
volatile bool tap_started;

// flag blocking output to speaker
volatile bool sound_locked;

// current metronome mode (0 = SOUND_MODE, 1=VIBRT_MODE)
volatile bool mode;

// lcd backlight timer (in miliseconds)
volatile uint16_t backlight_time_counter;

// indicates whether lcd backlight is on
volatile bool backlight_on;

// content to be displayed on LCD
char firstLineBuffer[LCD_SIZE + 1];

char secondLineBuffer[LCD_SIZE + 1];

volatile uint8_t pitch_index;