/*
 *  File name:  lib_lcd.h
 *  Date first: 12/31/2017
 *  Date last:  08/07/2019
 *
 *  Description: Library for Hitachi HD44780 LCD on STM8 architecture.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize LCD
 */

void lcd_init(void);

/*
 *  Set LCD cursor position by line and column
 *  in: line (0-3), column (0-19)
 */

void lcd_curs(char, char);

/*
 *  Write character to LCD (and alias function)
 */

void lcd_putc(char);

/*
 *  Write string to LCD (zero terminated)
 */

void lcd_puts(char *);

/*
 *  Following addition contributed by jackkum <jackkum@bk.ru> August 2019
 */

/*
 *  Clear lcd
 */
void lcd_clear(void);

/*
 *  Mode of lcd
 */
void lcd_mode(char mode);


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


