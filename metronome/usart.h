#ifndef __usart_h__
#define __usart_h__

#include <avr/io.h>

#ifndef BAUD     
#define BAUD 9600 
#endif

#define USART_HAS_DATA bit_is_set(UCSR0A, RXC0)
#define USART_READY bit_is_set(UCSR0A, UDRE0)

void usart_init(void);

void transmit_byte(uint8_t byte);

uint8_t receive_byte(void);

#endif //__usart_h__