/*
 *  File name:  lib_tm1638.h
 *  Date first: 06/08/2018
 *  Date last:  06/23/2018
 *
 *  Description: STM8 Library for TM1638 LED array and keys.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *  Some code copied from lib_max7219.h
 *
 ******************************************************************************
 *
 *  Initialize array
 *  in: type
 */

void tm1638_init(char);

#define TM1638_8	1	/* display with 8 keys and 8 LEDs */
#define TM1638_16	2	/* display with 16 keys, no LEDs */

/* NOTE: Support for the 16 key board is a compile time option */
#define TM1638_16_KEYS		/* comment out if you don't need 16 key support */

/*
 *  Write digit/char to unit
 *  in: digit/char (ASCII). Bit-7 is decimal point
 */

void tm1638_putc(char);

/*
 *  Write number/ASCII string to unit
 *  in: string (insert decimal point where desired.)
 */

void tm1638_puts(char *);

/*
 *  Push display changes to the LEDs (16 key board only).
 *  LEDs are not updated until this call.
 */ 
void tm1638_push(void);

/*
 *  Set cursor position
 *  in: column (0-7)
 */

void tm1638_curs(char);

/*
 *  Clear displays
 */

void tm1638_clear(void);

/*
 *  Set LED status (8 key board only)
 *  in:  LED number (0 to 7), new status (0=off, 1=red)
 */
void tm1638_setled(char, char);

/*
 *  Set LED intensity
 *  in: 0 (minimum) to 7 (maximum)
 */
void tm1638_bright(char);

/*
 *  Set LED blinking
 *  Alternates between regular and minimum brightness.
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void tm1638_blink(char);

/*
 *  Poll for key press and blinking
 *  (call every millisecond)
 */
void tm1638_poll(void);

/*
 *  Get key
 *  out: key char ("0123456789ABCDEF"), zero if none
 *  Bit-7 is low for key press, high for release.
 */
char tm1638_getc(void);

/*  Set alternate key mappings
 *  in:  key map has 8 or 16 characters for keys
 */
void tm1638_kmap(char *);

