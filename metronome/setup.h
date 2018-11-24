#ifndef __setup_h__
#define __setup_h__

#ifdef F_CPU
    #if F_CPU != 8000000
        #undef F_CPU
        #define F_CPU 8000000
    #endif
#endif

#ifndef BAUD
    #define BAUD 9600
#endif

#include <avr/io.h>

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

// SPI
#define SPI_POT_SS      PB2
#define SPI_POT_SS_PORT PORTB
#define SPI_POT_SS_PIN  PINB
#define SPI_POT_SS_DDR  DDRB

#define SPI_MOSI        PB3
#define SPI_MOSI_PORT   PORTB
#define SPI_MOSI_PIN    PINB
#define SPI_MOSI_DDR    DDRB

#define SPI_MISO        PB4
#define SPI_MISO_PORT   PORTB
#define SPI_MISO_PIN    PINB
#define SPI_MISO_DDR    DDRB

#define SPI_SCK         PB5
#define SPI_SCK_PORT    PORTB
#define SPI_SCK_PIN     PINB
#define SPI_SCK_DDR     DDRB

#define TAP_TIMER_INITIAL_OFFSET    0x83
#define MIN_TAP_INTERVAL            150u
#define MAX_TAP_INTERVAL            2000u

#define MIN_BPM             30u
#define MAX_BPM             400u
#define MIN_VOLUME          0u
#define MAX_VOLUME          100u
#define VOLUME_STEP         5u
#define DEFAULT_BPM         140u
#define DEFAULT_VOL         5u
#define DEFAULT_SIGNATURE   0u
#define G                   33u
#define CIS                 47u

#define BPM_CURSOR_POS      0u
#define SIG_CURSOR_POS      1u 
#define VOL_CURSOR_POS      2u
#define MODE_CURSOR_POS     3u
#define PARAM_EDIT_SYMBOL   '>'
#define PARAM_SELECT_SYMBOL '}'

#define SOUND_LABEL     "SOUND"
#define VIBRT_LABEL     "VIBRT"

#endif //__setup_h__