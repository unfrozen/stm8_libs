/*
 *  File name:  lib_lcd.h
 *  Date first: 12/31/2017
 *  Date last:  10/07/2018
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
