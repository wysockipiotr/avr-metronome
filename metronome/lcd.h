#ifndef __lcd_h__
#define __lcd_h__

#include <avr/io.h>

#define LCD_SIZE            16u
#define LCD_FIRST_LINE      0x80
#define LCD_SECOND_LINE     0xC0

// custom character codes
#define LCD_FAT_ARROW_CHAR  1
#define LCD_ARROWS_CHAR     2
#define LCD_SPEAKER_CHAR    3
#define LCD_WAVE_CHAR       4
#define LCD_NOTE_CHAR       5

void lcd_init(void);

void lcd_command(unsigned char cmnd);

void lcd_char(unsigned char data);

void lcd_custom_char(unsigned char location, const uint8_t * bytes);

void lcd_clear(void);

void lcd_string(char * str);

void lcd_string_xy(char row, char pos, char * str);

void load_tone_name_from_pgm(uint8_t tone_index, char * tone_name);

#endif //__lcd_h__