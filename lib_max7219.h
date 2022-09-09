/*
 *  File name:  lib_max7219.h
 *  Date first: 02/27/2018
 *  Date last:  03/16/2018
 *
 *  Description: STM8 Library for MAX7219 LED array.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize array
 *  in: type, device count
 */

void m7219_init(char, char);

#define MAX7219_7SEG	1	/* 8 7-segment LEDs */
#define MAX7219_DOT	2	/* 8x8 LED matrix(es), using 5x8 font */
#define MAX7219_GRAPH	3	/* 8x8 LED matrix(es), sending graphics */

/* Comment out or erase the definition for MAX7219_DOT if you don't use
 * the font bitmap. The font uses 480 bytes of program memory.
 * Both MAX7219_DOT and MAX7219_GRAPH use an extra 9 bytes of RAM.
 * Graphics are bit columns, left to right, lowest bit on top, bit-7 on bottom.
 */

/*
 *  Write digit/char to unit
 *  in: digit/char (ASCII)/graphic. If 7SEG, bit-7 is decimal point
 */

void m7219_putc(char);

/*
 *  Write number/ASCII string to unit
 *  in: string (Note: for graphics, zero is valid, so use m7219_putc() instead)
 */

void m7219_puts(char *);

/*
 *  Set cursor position
 *  in: line or device# (0-max), column
 */

void m7219_curs(char, char);

/*
 *  Clear all displays
 */

void m7219_clear(void);

/*
 *  Set LED intensity
 *  in: 0 (minimum) to 15 (maximum)
 */

void m7219_bright(char);

/*
 *  Set LED options
 */

void m7219_option(char);

#define MAX7219_WRAP	1	/* wrap around to first row & column */
#define MAX7219_NOWRAP	2	/* don't wrap, ignore extra data */
#define MAX7219_MARQUEE	3	/* begin marquee: don't show until wrap */

#define MAX7219_PRERENDER	/* Render into pixel buffer. */

#ifdef MAX7219_PRERENDER
/*
 *  Pre-render text into a raw pixel buffer instead of sending to the
 *  displays. This will make sending the pixels much faster. The buffer size
 *  is 8 bytes for every MAX7219 device (the 8x32 module has 4). The "skip"
 *  count  is the number of left pixels to omit from the first character.
 *  This is for making a moving marquee.
 *
 *  in:  pixel buffer, pixels to skip
 */
void m7219_prerender(char *, char);

/*
 *  Send pre-rendered pixel buffer to displays.
 *
 *  in: pixel buffer
 */
void m7219_sendbuf(char *);

#endif /* MAX7219_PRERENDER */
