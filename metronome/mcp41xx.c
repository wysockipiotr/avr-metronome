#include "mcp41xx.h"
#include <avr/io.h>

inline static void slave_select(void);

inline static void slave_deselect(void);

inline static void spi_trade_byte(uint8_t byte);

void spi_init(void) {
    // mcp41xx (slave) select as output
    setBit(SPI_POT_SS_DDR, SPI_POT_SS);

    // begin without selecting mcp41xx (high state)
    setBit(SPI_POT_SS_PORT, SPI_POT_SS);

    // set MOSI pin as output
    setBit(SPI_MOSI_DDR, SPI_MOSI);

    // set MISO pin as input with pull-up resistor
    setInputPullup(SPI_MISO_DDR, SPI_MISO_PORT, SPI_MISO);

    // spi clock line as output 
    setBit(SPI_SCK_DDR, SPI_SCK);

    // set most significant bit first (required for MCP41xx)
    clearBit(SPCR, DORD);

    // TODO: make sure about the polarity (CPOL, CPHA) 

    // /16 divider
    setBit(SPCR, SPR1);
    setBit(SPCR, MSTR); // main clock
    setBit(SPCR, SPE); // enable
}

inline static void slave_select(void) {
    clearBit(SPI_POT_SS_PORT, SPI_POT_SS);
}

inline static void slave_deselect(void) {
    setBit(SPI_POT_SS_PORT, SPI_POT_SS);
}

inline static void spi_trade_byte(uint8_t byte) {
    // begin byte exchange
    SPDR = byte;

    // wait until finished
    loop_until_bit_is_set(SPSR, SPIF);

    // SPDR register now contains received byte
}

void mcp_pot_set_percent_value(uint8_t percent) {
    uint16_t x = (255u * percent) / 100u;
    slave_select();

    // send write data command
    spi_trade_byte(0x0);

    // send 10kOhm potentiometer value (mapped to 0-255 range)
    spi_trade_byte(x);
    slave_deselect();
}