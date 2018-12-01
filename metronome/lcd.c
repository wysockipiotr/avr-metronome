#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"
#include "setup.h"
#include "util.h"

const static uint8_t arrow_char[] PROGMEM = {0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00};

const static uint8_t arrows_char[] PROGMEM = {0x04, 0x0E, 0x1F, 0x00, 0x00, 0x1F, 0x0E, 0x04};

const static uint8_t spkr_char[] PROGMEM = {0x02, 0x06, 0x1E, 0x1E, 0x1E, 0x1E, 0x06, 0x02};

const static uint8_t wave_char[] PROGMEM = {0x00, 0x01, 0x05, 0x15, 0x15, 0x05, 0x01, 0x00};

const static uint8_t note_char[] PROGMEM = {0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x1F, 0x0E};

void lcd_init(void) {
    uint8_t i = 0u;

    LCD_PORT = 0x0;
    LCD_DIR = 0xff; /* Make LCD port direction as o/p */
    _delay_ms(20);  /* LCD Power ON delay always >15ms */
    clearBit(LCD_CMD_PORT, EN);
    clearBit(LCD_CMD_PORT, RS);

    for (i = 0u; i < 3u; ++i) {
        setBit(LCD_CMD_PORT, EN);
        lcd_command(0x02); /* send for 4 bit initialization of LCD  */
        clearBit(LCD_CMD_PORT, EN);
        _delay_ms(5);
    }

    _delay_ms(1);

    lcd_command(0x28); /* 2 line, 5*7 matrix in 4-bit mode */
    lcd_command(0x0c); /* Display on cursor off*/
    lcd_command(0x06); /* Increment cursor (shift cursor to right)*/
    lcd_command(0x01); /* Clear display screen*/
    _delay_ms(2);

    // write custom symbols to CGRAM
    _delay_ms(20);
    lcd_custom_char(LCD_SPEAKER_CHAR, spkr_char);
    _delay_ms(20);
    lcd_custom_char(LCD_WAVE_CHAR, wave_char);
    _delay_ms(20);
    lcd_custom_char(LCD_NOTE_CHAR, note_char);
    _delay_ms(20);
    lcd_custom_char(LCD_ARROWS_CHAR, arrows_char);
    _delay_ms(20);
    lcd_custom_char(LCD_FAT_ARROW_CHAR, arrow_char);
}

void lcd_command(unsigned char cmnd) {
    // send upper nibble
    LCD_PORT = (LCD_PORT & 0xF0) | (cmnd >> 4);
    clearBit(LCD_PORT, RS);

    // enable pulse
    setBit(LCD_PORT, EN);
    _delay_us(1);
    clearBit(LCD_PORT, EN);

    _delay_us(200);

    // send lower nibble
    LCD_PORT = (LCD_PORT & 0xF0) | (cmnd & 0x0F);

    // enable pulse
    setBit(LCD_PORT, EN);
    _delay_us(1);
    clearBit(LCD_PORT, EN);

    _delay_ms(2);
}

void lcd_char(unsigned char data) {
    /* sending upper nibble */
    LCD_PORT = (LCD_PORT & 0xF0) | (data >> 4);
    LCD_PORT |= (1 << RS); /* RS=1, data reg. */

    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);

    _delay_us(200);

    // LCD_PORT = (LCD_PORT & 0x0F) | (data << 4); /* sending lower nibble */
    LCD_PORT = (LCD_PORT & 0xF0) | (data & 0x0F);

    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);

    _delay_ms(2);
}

void lcd_custom_char(uint8_t location, const uint8_t* bytes) {
    uint8_t i = 0u;
    uint8_t command = 0x40 | (location << 3);
    if (location < 8u) {
        lcd_command(command++);
        for (i = 0u; i < 8u; ++i) {
            lcd_char(pgm_read_byte(bytes + i));
        }
    }
}

void lcd_string(char* str) {
    int i;
    for (i = 0; str[i] != 0; i++) {
        /* Send each char of string till the NULL */
        lcd_char(str[i]);
    }
}

void lcd_string_xy(char row, char pos, char* str) {
    if (row == 0 && pos < 16)
        lcd_command((pos & 0x0F) | LCD_FIRST_LINE); /* Command of first row and required position<16 */
    else if (row == 1 && pos < 16)
        lcd_command((pos & 0x0F) | LCD_SECOND_LINE); /* Command of first row and required position<16 */
    lcd_string(str);                                 /* Call LCD string function */
}

void lcd_clear(void) {
    lcd_command(0x01); /* Clear display */
    _delay_ms(2);
    lcd_command(0x80); /* Cursor at home position */
}