#ifndef __setup_h__
#define __setup_h__

// #define SINE_WAVE

#define LCD_DIR         DDRC  
#define LCD_PORT        PORTC 
#define RS              PC4 
#define EN              PC5   

#define ROTARY_PORT     PORTD
#define ROTARY_BTN      PD2
#define ROTARY_PIN      PIND
#define ROTARY_DIR      DDRD
#define ROTARY_A        PD3
#define ROTARY_B        PD4

#define SPEAKER_DIR     DDRD
#define SPEAKER_PORT    PORTD
#define SPEAKER         PD6
#define SPEAKER_PIN     PIND

#define TAP_DIR         DDRD
#define TAP_PORT        PORTD
#define TAP_PIN         PIND
#define TAP_BTN         PD7

#define TAP_TIMER_INITIAL_OFFSET 0x83

#define DEFAULT_BPM         140u
#define DEFAULT_VOL         50u
#define DEFAULT_SIGNATURE   0

#endif //__setup_h__