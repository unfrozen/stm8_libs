/*
 *  File name:  lib_tm1637.c
 *  Date first: 06/08/2018
 *  Date last:  06/13/2019
 *
 *  Description: STM8 Library for TM1637 4 digit LED array.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *  Base code copied from lib_tm1638.c
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"
#include "lib_i2c.h"
#include "lib_tm1637.h"

/* Note that the I2C pins are defined in lib_i2c.c
 * Clock = D2, Data = D3
 */

#define COLON_POS	1	/* LED that has colon in bit-7 */

#define	SEG_INVALID	0x49	/* "invalid", three horizontal bars */

static char led_col;		/* digit 7-seg always 0-3 */
static char led_bright;		/* normal brightness */

static char blink_rate;		/* blink rate, time 1/100 second */
static char blink_count;
static char blink_ms;
static char blink_flags;

static char colon_bars;		/* bars for LED with colon bit */

static void emit_byte(char);	/* send byte */
static void emit_segs(char, char); /* send 7-segment to LED */

static char  code_7seg(char);	/* get 7-segment code for ASCII character */

const char segs_1637_digits[];
const char segs_1637_alpha[];

static volatile char lock;

/******************************************************************************
 *
 *  Initialize device
 */

void tm1637_init(void)
{
    led_col = 0;

    i2c_init();

    tm1637_bright(5);		/* active and brightness */
    tm1637_clear();

    lock = 0;
    blink_rate = 0;
}

/******************************************************************************
 *
 *  Set cursor position
 *  in: line or unit# (0-max), column
 */

void tm1637_curs(char col)
{
    led_col = col;
}

/******************************************************************************
 *
 *  Write number/string to unit
 *  in: number/string
 */

void tm1637_puts(char *str)
{
    char	c;
    while (c = *str++)
	tm1637_putc(c);
}

/******************************************************************************
 *
 *  Get 7-segment code for ASCII character
 */

static char code_7seg(char c)
{
    if (c == ' ')
	return 0;
    if (c == '-')
	return 0x40;
    if (c == '_')
	return 8;
    if ((c & 0xf0) == 0x30)
	return segs_1637_digits[c & 0x0f];
    if (c < 'A' ||
	c > 'U')
	return SEG_INVALID;
    return segs_1637_alpha[c - 'A'];
}

/******************************************************************************
 *
 *  Write 7-segment bars to LED
 *  in: position, segments
 */

static void emit_segs(char pos, char segs)
{
    lock = 1;

    i2c_start();
    emit_byte(0x44);		/* data write, no increment */
    i2c_getack();
    i2c_stop();

    i2c_start();
    emit_byte(0xc0 | pos);
    i2c_getack();
    emit_byte(segs);
    i2c_getack();
    i2c_stop();

    lock = 0;
}

/******************************************************************************
 *
 *  Write digit/char to unit
 *  in: digit or char
 */

void tm1637_putc(char c)
{
    char	pos, segs;

    pos = led_col;
    if (c == ':') {
	segs = colon_bars | 0x80;
	colon_bars = segs;
	emit_segs(COLON_POS, segs);/* re-write LED with colon */
	return;
    }
    pos = led_col;
    segs = code_7seg(c & 0x7f);
    if (pos == COLON_POS)
	    colon_bars = segs;
    emit_segs(pos, segs);

    led_col++;
    led_col &= 3;
}

/******************************************************************************
 *
 *  Clear all displays
 */

void tm1637_clear(void)
{
    tm1637_curs(0);
    do
	tm1637_putc(' ');
    while (led_col);
}

/******************************************************************************
 *
 *  Set LED intensity (and clear test mode)
 *  in:  0 (minimum) to 7 (maximum)
 */

void tm1637_bright(char intensity)
{
    lock = 1;

    led_bright = intensity;
    i2c_start();
    emit_byte(0x88 | intensity);
    i2c_getack();
    i2c_stop();

    lock = 0;
}

/******************************************************************************
 *
 *  Set the display colon
 *  in: 0 (off) not zero (on)
 */
void tm1637_colon(char on)
{
    char	new;

    new = colon_bars & 0x7f;
    if (on)
	new |= 0x80;
    emit_segs(COLON_POS, new);
    colon_bars = new;
}

/******************************************************************************
 *
 *  Set LED blinking
 *  Alternates between regular and minimum brightness.
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void tm1637_blink(char rate)
{
    blink_rate = rate;
    blink_count = rate;
    blink_flags = 0x80;
    blink_ms = 10;
    if (rate)
	return;
    tm1637_bright(led_bright);	/* blink off, back to normal */
}

/******************************************************************************
 *
 *  Poll for blink event
 *  (Call every millisecond)
 */

static void blink_check(void)
{
    char	command;

    if (!blink_rate)
	return;
    if ((blink_flags & 0x80) &&	/* pending brightness change? */
	!lock) {		/* and okay to write? */
	blink_flags ^= 0x80;
	command = 0x80;		/* display off */
	if (blink_flags & 1)
	    command |= 0x08 | led_bright;
	i2c_start();
	emit_byte(command);
	i2c_getack();
	i2c_stop();
    }
    blink_ms--;
    if (blink_ms)
	return;
    blink_ms = 10;
    blink_count--;
    if (blink_count)
	return;
    blink_count = blink_rate;
    blink_flags |= 0x80;	/* brightness change pending */
    blink_flags ^= 0x01;	/* normal or minimum brightness */
}

/******************************************************************************
 *
 *	 aaa		 000
 *	f   b		5   1
 *	f   b		5   1
 *	 ggg		 666
 *	e   c		4   2
 *	e   c		4   2
 *	 ddd  h		 333  7
 */

const char segs_1637_digits[16] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,	/* 0 - 4 */
    0x6d, 0x7d, 0x07, 0x7f, 0x67,	/* 5 - 9 */
    SEG_INVALID, SEG_INVALID, SEG_INVALID, 0x48, SEG_INVALID, SEG_INVALID
};
const char segs_1637_alpha[21] = {
    0x77,		/* A */
    0x7c,		/* B */
    0x39,		/* C */
    0x5e,		/* D */
    0x79,		/* E */
    0x71,		/* F */
    SEG_INVALID,	/* G */
    0x76,		/* H */
    0x06,		/* I shared with 1 */
    0x0e,		/* J */
    SEG_INVALID,	/* K */
    0x38,		/* L */
    SEG_INVALID,	/* M */
    0x54,		/* N (small n) */
    0x5c,		/* O (small o) */
    0x73,		/* P */
    SEG_INVALID,	/* Q */
    0x50,		/* R (small r) */
    0x6d,		/* S shared with 5 */
    0x78,		/* T (small t) */
    0x3e		/* U */
};

/******************************************************************************
 *
 *  Poll for blinking
 *  (every millisecond)
 */
void tm1637_poll(void)
{
    blink_check();
}

/******************************************************************************
 *
 *  Transmit 8 bits
 */

void emit_byte(char bits)
{
    bits;
__asm
    push	#8
00001$:
    srl		(4, sp)
    jrnc	00010$
    call	_i2c_data1
    call	_i2c_data1
    call	_i2c_data1
    jra		00020$
00010$:
    call	_i2c_data0
00020$:
    call	_i2c_clock1
    call	_i2c_clock1
    call	_i2c_clock0

    dec		(1, sp)
    jrne	00001$

    add		sp, #1
__endasm;
}
