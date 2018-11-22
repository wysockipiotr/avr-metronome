#ifndef __interrupts_h__
#define __interrupts_h__

#include "setup.h"

// setup and run metronome
void metronome(void);

// handle rotary encoder and tap tempo button interrupts
void handle_portd_pin_change(void);

// handle tap tempo timer overflow interrupt
void handle_timer2_overflow(void);

#endif //__interrupts_h__