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

static void setup(void);

static void loop(void);

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

static void updateActiveParam(int delta) {
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
    TIMSK2 = (1 << TOIE2);
}

inline static void init_sound_timer(void) {
    setBit(TCCR0A, WGM01);
    setBit(TCCR0A, COM0A0);
    setBit(TCCR0B, CS00);
    setBit(TCCR0B, CS01);
}

inline static void play_note(uint8_t wavelength, uint8_t duration) {
    OCR0A = wavelength;
    if (!sound_locked) {
        setBit(SPEAKER_DIR, SPEAKER);
        while (duration) {
            _delay_ms(1u);
            --duration;
        }
        clearBit(SPEAKER_DIR, SPEAKER);
    }
}

// IMPORTANT: this must be included right here
#include "interrupts.h"

int main(void) {
    setup();
    loop();
    return (0);
}

static void setup(void) {
    // clk / 1
    clock_prescale_set(CLK_NO_DIVIDE);

    // TODO Read stored settings from internal EEPROM
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

static void loop(void) {
    uint8_t i = 0u;
    while (true) {
        play_note(100u, t_beep);
        _delay_ms(t_sleep);
        for (i = 0u; i < (signatures[signature] >> 4) - 1; ++i) {
            play_note(120u, t_beep);
            _delay_ms(t_sleep);
        }
    }
}

