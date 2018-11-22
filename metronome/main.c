#include <avr/interrupt.h>

#include "lcd.h"
#include "metronome.h"
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

// index of time signature chosen from signatures array
volatile uint8_t signature;

// duration of metronome beep
volatile uint16_t t_beep;

// duration of silence between beeps
volatile uint16_t t_sleep;

// sound volume expressed in percent
volatile uint8_t volume;

// string literal indicating current mode: "SOUND" or "VIBRT"
volatile char* mode;

// counter measuring interval between last two taps in miliseconds
volatile uint16_t tap_interval_counter;

// flag indicating whether tap tempo mode is currently active
volatile bool tap_started;

// flag blocking output to speaker
volatile bool sound_locked;

// available time signatures
uint8_t signatures[4] = {0x44, 0x34, 0x54, 0x74};

// content to be displayed on LCD
char firstLineBuffer[LCD_SIZE + 1];

char secondLineBuffer[LCD_SIZE + 1];

ISR(PCINT2_vect) {
    handle_portd_pin_change();
}

ISR(TIMER2_OVF_vect) {
    handle_timer2_overflow();
}

int main(void) {
    metronome();
}