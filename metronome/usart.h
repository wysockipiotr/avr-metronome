#ifndef __usart_h__
#define __usart_h__

#include <avr/io.h>

#ifndef BAUD     
#define BAUD 9600 
#endif

#define USART_HAS_DATA bit_is_set(UCSR0A, RXC0)
#define USART_READY bit_is_set(UCSR0A, UDRE0)

// enable usart (8 data bits, no parity, 1 stop bit)
void usart_init(void);

// send single byte
void transmit_byte(uint8_t byte);

// send c string
void transmit_string(const char * str);

// send metronome values to the remote module
void transmit_metronome(uint16_t bpm, uint8_t signature);

// send vibrations-off signal to the remote module
void transmit_metronome_off(void);

uint8_t receive_byte(void);

#endif //__usart_h__