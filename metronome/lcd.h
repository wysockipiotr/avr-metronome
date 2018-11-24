#ifndef __lcd_h__
#define __lcd_h__

#define X 10


#define LCD_SIZE        16u
#define LCD_FIRST_LINE  0x80
#define LCD_SECOND_LINE 0xC0

void lcd_init(void);

void lcd_command(unsigned char cmnd);

void lcd_char(unsigned char data);

void lcd_clear(void);

void lcd_string(char * str);

void lcd_string_xy(char row, char pos, char * str);

#endif //__lcd_h__