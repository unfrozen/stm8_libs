/*
 *  File name:  lib_max7219.c
 *  Date first: 02/27/2018
 *  Date last:  03/05/2018
 *
 *  Description: STM8 Library for MAX7219 LED array.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *
 ******************************************************************************
 *
 */

#include "stm8_103.h"		/* C defines */
#include "stm8_103.inc"		/* asm defines */
#include "lib_max7219.h"

#define PORT_ODR PD_ODR
#define PORT_DDR PD_DDR
#define PORT_CR1 PD_CR1
#define _PORT_ODR _PD_ODR

#define PIN_CLK	 1
#define PIN_LOAD 2
#define PIN_DATA 3

#define	SEG_INVALID	0x49	/* "invalid", three horizontal bars */

static char led_type;
static char led_count;

static char led_row;		/* line or unit# */
static char led_col;		/* digit (7-seg) or dot column, always 0-7 */

static void emit_word(int);

const char segs_digits[];
const char segs_alpha[];

/******************************************************************************
 *
 *  Initialize array
 *  in: type, count
 */

void m7219_init(char type, char count)
{
    PORT_ODR &= 0xf1;		/* start pins low */
    PORT_DDR |= 0x0e;		/* D1, D2, D3 outputs */
    PORT_CR1 |= 0x0e;		/* enable source output */

    led_type  = type;
    led_count = count;
    led_row = 0;
    led_col = 0;

    emit_word(0x0900);		/* no BCD decode for any digits */
    emit_word(0x0b07);		/* scan all digits */
    m7219_bright(15);		/* maximum brightness */
    emit_word(0x0c01);		/* normal operation */

    m7219_clear();
}

/******************************************************************************
 *
 *  Set cursor position
 *  in: line or unit# (0-max), column
 */

void m7219_curs(char line, char col)
{
    led_row = line;
    led_col = col;
}

/******************************************************************************
 *
 *  Write number/string to unit
 *  in: number/string
 */

void m7219_puts(char *str)
{
    char	c;
    while (c = *str++) {
	if (*str == '.') {
	    c |= 0x80;
	    str++;
	}
	m7219_putc(c);
    }
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
	return 1;
    if (c == '_')
	return 8;
    if ((c & 0xf0) == 0x30)
	return segs_digits[c & 0x0f];
    if (c < 'A' ||
	c > 'U')
	return SEG_INVALID;
    return segs_alpha[c - 'A'];
}

/******************************************************************************
 *
 *  Write digit/char to unit
 *  in: digit or char (bit-7 for decimal point)
 */

void m7219_putc(char c)
{
    char	pos, segs;

    if (led_type == MAX7219_7SEG) {
	segs = code_7seg(c);
	segs |= (c & 0x80);
	pos = 8 - led_col;
	emit_word((pos << 8) | segs);

	led_col++;
	if (led_col & 8) {
	    led_col = 0;
	    led_row++;
	    if (led_row == led_count)
		led_row = 0;
	}
	return;
    }
    if (led_type == MAX7219_DOT) {

    }
}

/******************************************************************************
 *
 *  Clear all displays
 */

void m7219_clear(void)
{
    m7219_curs(0, 0);
    do
	m7219_putc(' ');
    while (led_col | led_row);
}

/******************************************************************************
 *
 *  Set LED intensity (and clear test mode)
 *  in:  0 (minimum) to 15 (maximum)
 */

void m7219_bright(char intensity)
{
    emit_word(0x0a00 | intensity);
}

/******************************************************************************
 *
 *  Send word to controller
 */

static void emit_word(int w)
{
    w;
__asm
    ldw		x, (3, sp)
    push	#16
00001$:
    bres	_PORT_ODR, #PIN_CLK
    rlcw	x
    bccm	_PORT_ODR, #PIN_DATA
    bset	_PORT_ODR, #PIN_CLK
    dec		(1, sp)
    jrne	00001$

    bset	_PORT_ODR, #PIN_LOAD
    bres	_PORT_ODR, #PIN_CLK
    bres	_PORT_ODR, #PIN_LOAD

    pop		a
__endasm;
}

/*	 aaa		 666
 *	f   b		1   5
 *	f   b		1   5
 *	 ggg		 000
 *	e   c		2   4
 *	e   c		2   4
 *	 ddd  h		 333  7
 */

const char segs_digits[16] = {
    0x7e, 0x30, 0x6d, 0x79, 0x33,	/* 0 - 4 */
    0x5b, 0x5f, 0x70, 0x7f, 0x7b,	/* 5 - 9 */
    SEG_INVALID, SEG_INVALID, SEG_INVALID, SEG_INVALID, SEG_INVALID, SEG_INVALID
};
const char segs_alpha[21] = {
    0x77,		/* A */
    0x1f,		/* B */
    0x4e,		/* C */
    0x3d,		/* D */
    0x4f,		/* E */
    0x47,		/* F */
    SEG_INVALID,	/* G */
    0x37,		/* H */
    SEG_INVALID,	/* I */
    0x38,		/* J */
    SEG_INVALID,	/* K */
    0x0e,		/* L */
    SEG_INVALID,	/* M */
    SEG_INVALID,	/* N */
    0x1d,		/* O (small o) */
    0x67,		/* P */
    SEG_INVALID,	/* Q */
    SEG_INVALID,	/* R */
    0x5b,		/* S shared with 5 */
    SEG_INVALID,	/* T */
    0x3e		/* U */
};