#ifndef __mcp41xx_h__
#define __mcp41xx_h__

#include <avr/io.h>
#include "setup.h"
#include "util.h"

// prepare SPI interface for communication with MCP41xx chip
void spi_init(void);

// adjust potentiometer digital wiper "position"
void mcp_pot_set_percent_value(uint8_t percent);

#endif //__mcp41xx_h__