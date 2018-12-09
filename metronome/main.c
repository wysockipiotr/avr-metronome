#include <avr/interrupt.h>

#include "lcd.h"
#include "metronome.h"
#include "util.h"
#include "global.h"

ISR(PCINT2_vect) {
    handle_portd_pin_change();
}

ISR(TIMER2_OVF_vect) {
    handle_timer2_overflow();
}

int main(void) {
    metronome();
}