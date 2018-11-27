#include <avr/io.h>
#include <util/setbaud.h>

#include "usart.h"
#include "util.h"

void usart_init(void) {  

    // TODO: this should be based on settings
    UBRR0H = 0;  //UBRRH_VALUE; 
    UBRR0L = 103u;//UBRRL_VALUE; 8Mhz clk, 9600 baudrate

#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif

    // parity disable
    clearBit(UCSR0C, UPM00);
    clearBit(UCSR0C, UPM01);

    // set 1 stop bit
    clearBit(UCSR0C, USBS0);
    
    // enable RX and TX
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    
    // 8 data bits
    clearBit(UCSR0C, UCSZ02);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void transmit_byte(uint8_t byte) {
    
    // wait for empty transmission buffer
    loop_until_bit_is_set(UCSR0A, UDRE0);

    // store byte in register to send it
    UDR0 = byte; 
}

void transmit_string(const char* str) {
    uint8_t i = 0u;
    while (str[i]) {
        transmit_byte(str[i++]);
    }
}

void transmit_metronome(uint16_t bpm, uint8_t signature) {

    // transmit bpm upper half
    transmit_byte(bpm >> 8);

    // transmit bpm lower half
    transmit_byte(bpm & 0xff);

    // transmit signature (e.g. 0010 0011 for 3/4 signature)
    transmit_byte(signature);
}

void transmit_metronome_off(void) {

    // transmit vibration disable signal
    transmit_byte(0x0);
}

uint8_t receive_byte(void) {
    
    // wait for the incoming byte
    loop_until_bit_is_set(UCSR0A, RXC0); 
    
    // return received byte from the register
    return UDR0;                    
}