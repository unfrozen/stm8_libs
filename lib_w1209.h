/*
 *  File name:  lib_w1209.h
 *  Date first: 01/22/2019
 *  Date last:  02/23/2019
 *
 *  Description: STM8 Library for W1209 thermostat board.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2019 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Configuration
 */

#define KB_SIZE		4	/* size of key buffer */
#define KB_DEBOUNCE	40	/* debounce count in milliseconds */
#define LED_UPDATE	5	/* milliseconds between LED updates */

/*
 *  Initialize board
 */

void w12_init(void);

/*
 *  Write digit/char to LED
 *  in: ASCII digit or char (use bit-7 for decimal point)
 */

void w12_putc(char);

/*
 *  Write string to LED
 *  in: ASCII string (insert decimal point where desired.)
 */

void w12_puts(char *);

/*
 *  Set cursor position
 *  in: column (0-2)
 */

void w12_curs(char);

/*
 *  Set LED blinking
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void w12_blink(char);

/*
 *  Get keypress
 *  out: zero or ASCII key ('0' to '2'), bit-7 set if released
 */

char w12_getc(void);

/*
 *  Poll for display, keys, and blinking
 *  (call every millisecond)
 */
void w12_poll(void);

/*
 *  Turn the relay on/off
 *  in: zero = off, non-zero = on
 */

void w12_relay(char);

/*
 *  Get the current temperature probe value
 *  out: current value, degrees C
 */

short w12_probe(void);
