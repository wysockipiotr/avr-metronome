#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "setup.h"
#include "util.h"

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

void lcd_init(void) {
    DDRC = 0xFF;   /* Make LCD port direction as o/p */
    _delay_ms(20); /* LCD Power ON delay always >15ms */

    lcd_command(0x02); /* send for 4 bit initialization of LCD  */
    lcd_command(0x28); /* 2 line, 5*7 matrix in 4-bit mode */
    lcd_command(0x0c); /* Display on cursor off*/
    lcd_command(0x06); /* Increment cursor (shift cursor to right)*/
    lcd_command(0x01); /* Clear display screen*/
    _delay_ms(2);
}

void lcd_string(char *str) {
    int i;
    for (i = 0; str[i] != 0; i++) /* Send each char of string till the NULL */
    {
        lcd_char(str[i]);
    }
}

void lcd_string_xy(char row, char pos, char *str) {
    if (row == 0 && pos < 16)
        lcd_command((pos & 0x0F) | LCD_FIRST_LINE); /* Command of first row and required position<16 */
    else if (row == 1 && pos < 16)
        lcd_command((pos & 0x0F) | LCD_SECOND_LINE); /* Command of first row and required position<16 */
    lcd_string(str);                      /* Call LCD string function */
}

void lcd_clear(void) {
    lcd_command(0x01); /* Clear display */
    _delay_ms(2);
    lcd_command(0x80); /* Cursor at home position */
}