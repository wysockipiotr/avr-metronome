#ifndef __interrupts_h__
#define __interrupts_h__

ISR(PCINT2_vect) {

    clearBit(SPEAKER_DIR, SPEAKER);
    //bool btnPushed = bit_is_clear(ROTARY_PIN, ROTARY_BTN);

    if (bit_is_clear(ROTARY_PIN, ROTARY_BTN)) {
        edit_active = !edit_active;
        cursor_symbol = (edit_active) ? '}' : '>';
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

        t_beep = 6000u / bpm;
        t_sleep = 54000u / bpm;

        update_display();
        // force a little down time before continuing
        _delay_ms(2);
        // wait until R1 comes back high
        while (bit_is_clear(ROTARY_PIN, ROTARY_A));
    }

    if (bit_is_clear(TAP_PIN, TAP_BTN)) {
        edit_active = false;
        cursor_symbol = (edit_active) ? '}' : '>';

        sound_locked = false;
        play_note(120u, t_beep);
        sound_locked = true;
        // tap tempo handler
        if (tap_started) {
            if (tap_interval_counter < 2001u && tap_interval_counter > 130u) {
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

// tap tempo timer (1ms) overflow
ISR(TIMER2_OVF_vect) {
    if (tap_interval_counter < 2001u) {
        ++tap_interval_counter;
        TCNT2 = TAP_TIMER_INITIAL_OFFSET;
    } else {
        // dismiss tap tempo in case of timeout (over 2000ms)
        tap_started = false;
        TCCR2B &= ~(TCCR2B & 0x7);
        sound_locked = false;
    }
}

#endif //__interrupts_h__