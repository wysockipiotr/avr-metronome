#include "util.h"
#include "global.h"
#include "usart.h"
#include "mcp41xx.h"
#include "metronome.h"

#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdio.h>
#include <util/delay.h>

// load initial settings, setup peripherals and ports
inline static void setup(void);

// enable interrupts, specify pin change interrupts mask
inline static void init_interrupts(void);

// init timer2 (tap tempo timer), enable its overflow interrupt
inline static void init_tap_timer(void);

// init timer0 (sound synthesize timer)
inline static void init_sound_timer(void);

// main metronome action loop
inline static void loop(void);

// play note with a given wavelength for a given duration
inline static void play_note(uint16_t wavelength, uint8_t duration);

// refresh displayed values and cursor arrow position
inline static void update_display(void);

// update value of the currently selected parameter by the given delta
inline static void update_active_param(int delta);

// recalc beep and silence durations regarding current bpm value
inline static void recalc_durations(void);

void metronome(void) {
    setup();
    loop();
}

void handle_portd_pin_change(void) {

    clearBit(SPEAKER_DIR, SPEAKER);

    if (bit_is_clear(ROTARY_PIN, ROTARY_BTN)) {
        edit_active = !edit_active;
        cursor_symbol = (edit_active) ? PARAM_EDIT_SYMBOL : PARAM_SELECT_SYMBOL;
        update_display();
    }

    if (bit_is_clear(ROTARY_PIN, ROTARY_A)) {

        if (bit_is_clear(ROTARY_PIN, ROTARY_B)) {

            // left rotate
            if (!edit_active) {
                if (cursor) {
                    --cursor;
                } else {
                    cursor = 3u;
                }
            } else {
                update_active_param(-1);
            }
        } else {

            // right rotate
            if (!edit_active) {
                ++cursor;
                cursor %= 4u;
            } else {
                update_active_param(1);
            }
        }

        recalc_durations();

        update_display();
        // force a little down time before continuing
        _delay_ms(2);
        // wait until R1 comes back high
        while (bit_is_clear(ROTARY_PIN, ROTARY_A)) {}
    }

    if (bit_is_clear(TAP_PIN, TAP_BTN)) {
        edit_active = false;
        cursor_symbol = (edit_active) ? PARAM_EDIT_SYMBOL : PARAM_SELECT_SYMBOL;

        sound_locked = false;
        play_note(120u, t_beep);
        sound_locked = true;

        // tap tempo handler
        if (tap_started) {
            if (within(tap_interval_counter, MIN_TAP_INTERVAL, MAX_TAP_INTERVAL)) {
                t_beep = tap_interval_counter >> 4;
                t_sleep = tap_interval_counter - t_beep;
                bpm = 60000u / tap_interval_counter;
                update_display();
            }
        } else {
            tap_started = true;
            sound_locked = true;
        }
        // start counting
        tap_interval_counter = 0u;
        //TCCR2A = 0;
        TCCR2B = 0x4;
        TCNT2 = TAP_TIMER_INITIAL_OFFSET;  // 131 ticks until overflow
    }
}

void handle_timer2_overflow(void) {
    if (tap_interval_counter <= MAX_TAP_INTERVAL) {
        ++tap_interval_counter;
        TCNT2 = TAP_TIMER_INITIAL_OFFSET;
    } else {
        // dismiss tap tempo in case of timeout (over 2000ms)
        tap_started = false;
        TCCR2B &= ~(TCCR2B & 0x7);
        sound_locked = false;
    }
}

inline static void update_display(void) {
    snprintf(firstLineBuffer, 17, "%c %3uBPM %c %2u/%1u ",
             cursorVisible(cursor, 0, cursor_symbol),
             bpm,
             cursorVisible(cursor, 1, cursor_symbol),
             signatures[signature] >> 4,
             signatures[signature] & 0x0f);
    snprintf(secondLineBuffer, 17, "%c %3u%%  %c  %s",
             cursorVisible(cursor, 2, cursor_symbol),
             volume,
             cursorVisible(cursor, 3, cursor_symbol),
             mode);
    lcd_string_xy(0, 0, firstLineBuffer);
    lcd_string_xy(1, 0, secondLineBuffer);
}

inline static void update_active_param(int delta) {
    switch (cursor) {
        case BPM_CURSOR_POS:
            if (within(bpm + delta, MIN_BPM, MAX_BPM)) {
                bpm += delta; 
            }
            break;
        case SIG_CURSOR_POS:
            signature += delta;
            signature %= 4;
            break;
        case VOL_CURSOR_POS:
            delta *= 5;
            if (within(volume + delta, MIN_VOLUME, MAX_VOLUME)) {
                volume += delta; 
            }
            mcp_pot_set_percent_value(volume);
            break;
        case MODE_CURSOR_POS:
            mode = (mode == SOUND_LABEL) ? VIBRT_LABEL : SOUND_LABEL;
            break;
        default:
            break;
    }
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
    mode = SOUND_LABEL;
    edit_active = false;
    cursor_symbol = PARAM_SELECT_SYMBOL;

    lcd_init();
    spi_init();
    usart_init();

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
    uint16_t t = 0u;

    while (true) {
        play_note(G, t_beep);
        for (t = t_sleep; t > 0; --t)
            _delay_ms(1);
        for (i = 0u; i < (signatures[signature] >> 4) - 1; ++i) {
            play_note(CIS, t_beep);
            for (t = t_sleep; t > 0; --t)
                _delay_ms(1);
        }
    }
}

inline static void init_interrupts(void) {
    // enable pin change interrupts on PORTD
    PCICR |= (1 << PCIE2);

    // pin change interrupt mask
    PCMSK2 |= ((1 << ROTARY_A) | (1 << ROTARY_B) | (1 << TAP_BTN) | (1 << ROTARY_BTN));

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

inline static void recalc_durations(void) {
    t_beep = 6000u / bpm;
    t_sleep = 54000u / bpm;
}

inline static void play_note(uint16_t wavelength, uint8_t duration) {
    OCR0A = wavelength;
    if (!sound_locked && !edit_active) {
        setBit(SPEAKER_DIR, SPEAKER);
        while (duration) {
            _delay_ms(1u);
            --duration;
        }
        clearBit(SPEAKER_DIR, SPEAKER);
    }
}