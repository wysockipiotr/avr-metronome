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
#include "lcd.h"

#define LCD_DIR                     DDRC  
#define LCD_DATA_DIR                DDRC
#define LCD_CMD_DIR                 DDRC
#define LCD_DATA_PORT               DDRC 
#define LCD_CMD_PORT                PORTC
#define LCD_PORT                    PORTC 
#define RS                          PC4 
#define EN                          PC5
#define LCD_D4                      PC0 
#define LCD_D5                      PC1   
#define LCD_D6                      PC2   
#define LCD_D7                      PC3   

#define ROTARY_PORT                 PORTD
#define ROTARY_BTN                  PD2
#define ROTARY_PIN                  PIND
#define ROTARY_DIR                  DDRD
#define ROTARY_A                    PD3
#define ROTARY_B                    PD4

#define SPEAKER_DIR                 DDRD
#define SPEAKER_PORT                PORTD
#define SPEAKER                     PD6
#define SPEAKER_PIN                 PIND

#define TAP_DIR                     DDRD
#define TAP_PORT                    PORTD
#define TAP_PIN                     PIND
#define TAP_BTN                     PD7

#define SHUTDOWN_DIR                DDRB
#define SHUTDOWN_PORT               PORTB
#define SHUTDOWN_PIN                PINB
#define SHUTDOWN                    PB0

#define BACKLIGHT_DIR               DDRB
#define BACKLIGHT_PORT              PORTB
#define BACKLIGHT_PIN               PINB
#define BACKLIGHT                   PB1

// SPI
#define SPI_POT_SS                  PB2
#define SPI_POT_SS_PORT             PORTB
#define SPI_POT_SS_PIN              PINB
#define SPI_POT_SS_DDR              DDRB

#define SPI_MOSI                    PB3
#define SPI_MOSI_PORT               PORTB
#define SPI_MOSI_PIN                PINB
#define SPI_MOSI_DDR                DDRB

#define SPI_MISO                    PB4
#define SPI_MISO_PORT               PORTB
#define SPI_MISO_PIN                PINB
#define SPI_MISO_DDR                DDRB

#define SPI_SCK                     PB5
#define SPI_SCK_PORT                PORTB
#define SPI_SCK_PIN                 PINB
#define SPI_SCK_DDR                 DDRB

#define TAP_TIMER_INITIAL_OFFSET    0x83
#define MIN_TAP_INTERVAL            150u
#define MAX_TAP_INTERVAL            2000u

#define BACKLIGHT_DURATION          20000u  // 20s
#define DEFAULT_BRIGHTNESS          120u

#define MIN_BPM                     30u
#define MAX_BPM                     400u
#define MIN_VOLUME                  0u
#define MAX_VOLUME                  100u
#define VOLUME_STEP                 5u
#define DEFAULT_BPM                 140u
#define DEFAULT_VOL                 5u
#define DEFAULT_SIGNATURE           0u
#define G                           33u
#define CIS                         47u
#define TAP_NOTE                    120u
#define TAP_BEEP_DURATION           100u

#define BPM_CURSOR_POS              0u
#define SIG_CURSOR_POS              1u 
#define VOL_CURSOR_POS              2u
#define MODE_CURSOR_POS             3u
#define PARAM_EDIT_SYMBOL           LCD_ARROWS_CHAR
#define PARAM_SELECT_SYMBOL         LCD_FAT_ARROW_CHAR

#define SOUND_LABEL                 "SOUND"
#define VIBRT_LABEL                 "VIBRT"

#define SOUND_MODE                  0u 
#define VIBRT_MODE                  1u
#define DEFAULT_MODE                SOUND_MODE

#define NUMBER_OF_MODES             2
#define NUMBER_OF_SIGNATURES        4

#define EEPROM_STORE_ADR            0
#define EEPROM_STORE_BPM            ((uint16_t*)EEPROM_STORE_ADR)
#define EEPROM_STORE_VOL            ((uint8_t*)EEPROM_STORE_ADR + 2)
#define EEPROM_STORE_SIG            ((uint8_t*)EEPROM_STORE_ADR + 3)
#define EEPROM_STORE_MODE           ((uint8_t*)EEPROM_STORE_ADR + 4)

#endif //__setup_h__