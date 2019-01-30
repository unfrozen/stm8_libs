/*
 *  File name:  lib_tm1638.h
 *  Date first: 06/08/2018
 *  Date last:  06/30/2018
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
 *  Initialize LED array
 */

void tm1637_init();

/*  The cheap 4 digit display has a colon between the first 2 and last 2 digits.
 *  If your string has a colon after the first 2 digits, the colon will display.
 *  Otherwise, the colon will be blank. You can also turn the colon on and off
 *  with the tm1637_colon() function below.
 */

/*
 *  Write digit/char to unit
 *  in: digit/char (ASCII).
 */

void tm1637_putc(char);

/*
 *  Write number/ASCII string to unit
 *  in: string (insert decimal point where desired.)
 */

void tm1637_puts(char *);

/*
 *  Set cursor position
 *  in: column (0-3)
 */

void tm1637_curs(char);

/*
 *  Clear display
 */

void tm1637_clear(void);

/*
 *  Set the display colon
 *  in: 0 (off) not zero (on)
 */
void tm1637_colon(char);

/*
 *  Set LED intensity
 *  in: 0 (minimum) to 7 (maximum)
 */
void tm1637_bright(char);

/*
 *  Set LED blinking
 *  Alternates between regular and minimum brightness.
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void tm1637_blink(char);

/*
 *  Poll for blinking
 *  (call every millisecond)
 */
void tm1637_poll(void);
