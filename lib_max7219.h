/*
 *  File name:  lib_max7219.h
 *  Date first: 02/27/2018
 *  Date last:  03/02/2018
 *
 *  Description: STM8 Library for MAX7219 LED array.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *
 ******************************************************************************
 *
 *  Initialize array
 *  in: type, count
 */

void m7219_init(char, char);

#define MAX7219_7SEG	1	/* unit with 8 7-segment LEDs */
#define MAX7219_DOT	2	/* unit with 8x8 LED matrix */

/*
 *  Write digit/char to unit
 *  in: digit/char (ASCII), set bit-7 for decimal point
 */

void m7219_putc(char);

/*
 *  Write number/string to unit
 *  in: number/string
 */

void m7219_puts(char *);

/*
 *  Set cursor position
 *  in: line or unit# (0-max), column
 */

void m7219_curs(char, char);

/*
 *  Set test mode for unit
 *  in: unit# (0-max)
 */

void m7219_test(char);

/*
 *  Set LED intensity (and clear test mode)
 *  in: 0 (minimum) to 15 (maximum)
 */

void m7219_bright(char);
