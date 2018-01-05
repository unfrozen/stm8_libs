/*
 *  File name:  lib_lcd.h
 *  Date first: 12/31/2017
 *  Date last:  12/31/2017
 *
 *  Description: Library for Hitachi LM016L LCD on STM8 architecture.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize LCD
 */

void lcd_init(void);

/*
 *  Set LCD cursor position (0x00-0x0f first line, 0x40-0x4f second line
 */

void lcd_curs(char);

/*
 *  Write character to LCD
 */

void lcd_put(char);

/*
 *  Write string to LCD (zero terminated)
 */

void lcd_puts(char *);
