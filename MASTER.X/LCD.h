/* 
 * File:   LCD.h
 * Author: debor
 *
 * Created on February 5, 2020, 9:12 AM
 */

#ifndef LCD_H
#define	LCD_H
#define _XTAL_FREQ   8000000
#define RS           RC1
#define RW           RC2
#define E            RC0
#define RS_T         TRISCbits.TRISC1
#define RW_T         TRISCbits.TRISC2
#define E_T          TRISCbits.TRISC0
#define LCD_PORT     PORTD
#define LCD_TRIS     TRISD

void LCD_COMANDO(char cmd);
void LCD_Cadena(char *data);
void LCD_INIT(void);
void LCD_Data(char data);
void LCD_XY(char x,char y);
void Lcd_Clear(void);

#endif	/* LCD_H */

