#define F_CPU 8000000

#ifndef BAUD
#define BAUD 9600
#endif

#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "util.h"
#include "setup.h"
#include "lcd.h"
#include "sound.h"

volatile uint8_t    cursor;
volatile uint16_t   bpm;
volatile uint8_t    signature; // e.g. 0100 0100 for 4/4 time signature
volatile uint16_t   t_beep;
volatile uint16_t   t_sleep;
volatile uint8_t    volume;
volatile uint16_t   tap_interval_counter;
volatile bool       tap_started;
volatile bool       sound_locked;
uint8_t             signatures[2] = { 0x44, 0x34 };
char                firstLineBuffer[LCD_SIZE+1];
char                secondLineBuffer[LCD_SIZE+1];

// load initial settings, setup peripherals and ports
inline static void setup(void);

// main metronome action loop
inline static void loop(void);

// refresh displayed values and cursor arrow position
inline static void update_display(void);

// update value of the currently selected parameter by the given delta
inline static void update_active_param(int delta);

// enable interrupts, specify pin change interrupts mask
inline static void init_interrupts(void);

// init timer2 (tap tempo timer), enable its overflow interrupt 
inline static void init_tap_timer(void);

// init timer0 (sound synthesize timer)
inline static void init_sound_timer(void);

// IMPORTANT: this must be included right here
#include "interrupts.h"

int main(void) {
    setup();
    loop();
}

inline static void setup(void) {
    // clk / 1
    clock_prescale_set(CLK_NO_DIVIDE);

    // initial values
    cursor = 0u;
    bpm = DEFAULT_BPM;
    volume = DEFAULT_VOL;
    signature = DEFAULT_SIGNATURE;
    t_beep = 6000u / bpm;
    t_sleep = 54000u / bpm;
    tap_interval_counter = 0u;
    tap_started = false;
    sound_locked = false;

    lcd_init();

    init_tap_timer();
    init_sound_timer();
    init_interrupts();

    setInputPullup(ROTARY_DIR, ROTARY_PORT, ROTARY_A);
    setInputPullup(ROTARY_DIR, ROTARY_PORT, ROTARY_B);
    setInputPullup(ROTARY_DIR, ROTARY_PORT, ROTARY_BTN);
    setInputPullup(TAP_DIR, TAP_PORT, TAP_BTN);

    update_display();
}

inline static void loop(void) {
    uint8_t i = 0u;
    while (true) {
        play_note(784u, t_beep);
        _delay_ms(t_sleep);
        for (i = 0u; i < (signatures[signature] >> 4) - 1; ++i) {
            play_note(554u, t_beep);
            _delay_ms(t_sleep);
        }
    }
}

inline static void update_display(void) {
    snprintf(firstLineBuffer, 17, "%c %3uBPM %c %2u/%1u ",
             cursorVisible(cursor, 0),
             bpm,
             cursorVisible(cursor, 1),
             signatures[signature] >> 4,
             signatures[signature] & 0x0f);
    snprintf(secondLineBuffer, 17, "%c %3u%%  %c  %s",
             cursorVisible(cursor, 2),
             volume,
             cursorVisible(cursor, 3),
             "SOUND");
    lcd_string_xy(0, 0, firstLineBuffer);
    lcd_string_xy(1, 0, secondLineBuffer);
}

inline static void update_active_param(int delta) {
    switch (cursor) {
        case 0u:
            if (bpm > 80u && bpm < 350u)
                bpm += delta;
            break;
        case 1u:
            signature += delta;
            signature %= 2;
            break;
        case 2u:
            if (volume > 0u && volume < 100u)
                volume += delta;
            break;
        case 3u:
            break;
        default:
            break;
    }
}

inline static void init_interrupts(void) {
    // enable pin change interrupts on PORTD
    PCICR |= (1 << PCIE2);

    // pin change interrupt mask
    PCMSK2 |= ((1 << ROTARY_A) | (1 << ROTARY_B) | (1 << TAP_BTN));

    // enable interrupts globally
    sei();
}

inline static void init_tap_timer(void) {
    // enable timer2 overflow interrupt
    TIMSK2 = (1 << TOIE2);
}

inline static void init_sound_timer(void) {
#ifdef SINE_WAVE
    TCCR0A |= (1 << COM0A1);       /* wyjście PWM z rejestru OCR0A */
    SPEAKER_DIR |= (1 << SPEAKER); /* włącz pin wyjściowy */

    TCCR0A |= (1 << WGM00); /* szybki tryb PWM */
    TCCR0A |= (1 << WGM01); /* szybki tryb PWM, część 2 */

    TCCR0B |= (1 << CS00);
#else
    setBit(TCCR0A, WGM01);
    setBit(TCCR0A, COM0A0);
    setBit(TCCR0B, CS00);
    setBit(TCCR0B, CS01);
#endif
}