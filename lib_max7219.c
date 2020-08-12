/*
 *  File name:  lib_max7219.c
 *  Date first: 02/27/2018
 *  Date last:  08/12/2020
 *
 *  Description: STM8 Library for MAX7219 LED array.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018, 2020 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"
#include "lib_max7219.h"
#include "lib_max7219.font"

#define PORT_ODR PD_ODR
#define PORT_DDR PD_DDR
#define PORT_CR1 PD_CR1
#define _PORT_ODR _PD_ODR

#define PIN_CLK	 1		/* D1 is module CLK */
#define PIN_LOAD 2		/* D2 is module CS/LOAD */
#define PIN_DATA 3		/* D3 is module DIN */

#define	SEG_INVALID	0x49	/* "invalid", three horizontal bars */

static char led_type;
static char led_count;

static char led_row;		/* line or unit# */
static char led_col;		/* digit (7-seg) or dot column, always 0-7 */

static char opt_wrap;		/* wrap around to first row? */
static char opt_marquee;	/* begin marquee */

static void emit_all(int);	/* issue command to all units */
static void emit_word(int);	/* issue command word and load */
static void emit_part(int);	/* not last word */
static void emit_load(void);	/* issue load command */
static void send_digit(int);	/* send digit/line to display */

static char  code_7seg(char);	/* get 7-segment code for ASCII character */
static char *code_dots(char);	/* get bitmap for ASCII character */

const char segs_digits[];
const char segs_alpha[];

#if (MAX7219_DOT) || (MAX7219_GRAPH)
static void send_grcol(char);	/* send graphic column */
static void send_graph(void);	/* send 8x8 graphics to unit */
static void zero_cache(void);	/* clear the graphics cache */

static char graph_cache[8];
static char graph_pixel;
#endif

/******************************************************************************
 *
 *  Initialize array
 *  in: type, device count
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
    opt_wrap = 1;
    opt_marquee = 0;

    emit_all(0x0900);		/* no BCD decode for any digits */
    emit_all(0x0b07);		/* scan all digits */
    m7219_bright(6);		/* brightness */
    emit_all(0x0c01);		/* normal operation */

    m7219_clear();
#if (MAX7219_DOT) || (MAX7219_GRAPH)
    graph_pixel = 1;
    zero_cache();
#endif
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
#if (MAX7219_DOT) || (MAX7219_GRAPH)
    graph_pixel = 1 << col;
    zero_cache();
#endif
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
    int		offset;
    char	*bm, bw;	/* char bitmap and width */

    if (led_type == MAX7219_7SEG) {
	segs = code_7seg(c & 0x7f);
	segs |= (c & 0x80);
	pos = 8 - led_col;
	send_digit((pos << 8) | segs);
	return;
    }
#ifdef MAX7219_DOT
    if (led_type == MAX7219_DOT) {
	c -= 32;
	if (c > 95)
	    c = 95;
	offset = c * 5;
	bm = font_map + offset;
	bw = 5;
	while (bw--)
	    send_grcol(*bm++);
	send_grcol(0);		/* space between chars */
	if (led_col)
	    send_graph();
	return;
    }
#endif
#ifdef MAX7219_GRAPH
    if (led_type == MAX7219_GRAPH) {
	send_grcol(c);
	if (led_col)
	    send_graph();
	return;
    }
#endif
}

/* 7-Segment unit positions: 8 7 6 5 4 3 2 1
 *
 * Dot matrix positions for horizontal (row/digit# and bit):
 *
 * 8: 0 1 2 3 4 5 6 7
 * 7: 0 1 2 3 4 5 6 7
 * 6: 0 1 2 3 4 5 6 7
 * 5: 0 1 2 3 4 5 6 7
 * 4: 0 1 2 3 4 5 6 7
 * 3: 0 1 2 3 4 5 6 7
 * 2: 0 1 2 3 4 5 6 7
 * 1: 0 1 2 3 4 5 6 7  <= graph_cache[0]
 * 
 ******************************************************************************
 *
 *  Send digit or dot column to display
 *  in: word with digit# and segs/dots
 */

static void send_digit(int digit)
{
    int		i;

    for (i = led_count; i > 0; i--)
	if (i == led_row + 1)
	    emit_part(digit);
	else
	    emit_part(0);
    emit_load();

    led_col++;
    if (led_col & 8) {
	led_col = 0;
	led_row++;
	if (led_row == led_count)
	    if (opt_wrap)
		led_row = 0;
    }
}

#if (MAX7219_DOT) || (MAX7219_GRAPH)
/******************************************************************************
 *
 *  Send graphics column to 8x8 graphics cache
 *  in 8-bit column
 */

static void send_grcol(char dots)
{
    char	i, mask;
    char	*cache;

    cache = graph_cache;
    mask  = ~graph_pixel;

    if (!opt_marquee)
	for (i = 0; i < 8; i++) {
	    *cache &= mask;
	    if (dots & 0x80)
		*cache |= graph_pixel;
	    cache++;
	    dots <<= 1;
	}
    graph_pixel <<= 1;
    led_col++;
    if (led_col & 8) {
	if (!opt_marquee)
	    send_graph();
	graph_pixel = 1;
	led_col = 0;
	led_row++;
	zero_cache();
	if (led_row == led_count) {
	    if (opt_wrap | opt_marquee)
		led_row = 0;
	    opt_marquee = 0;
	}
    }
}

/******************************************************************************
 *
 *  Send 8x8 graphics cache to display
 */

static void send_graph(void)
{
    char	 row, i;
    char	*cache;
    int		 digit;

    cache = graph_cache;
    for (row = 1; row <= 8; row++) {
	digit = *cache++;
	digit |= row << 8;
	for (i = led_count; i > 0; i--)
	    if (i == led_row + 1)
		emit_part(digit);
	    else
		emit_part(0);
	emit_load();
    }
}

/******************************************************************************
 *
 *  Clear the graphics cache
 */

static void zero_cache(void)
{
__asm
    ldw		x, #_graph_cache
    clr		(x)
    clr		(1, x)
    clr		(2, x)
    clr		(3, x)
    clr		(4, x)
    clr		(5, x)
    clr		(6, x)
    clr		(7, x)
__endasm;
}

#endif	/* (MAX7219_DOT) || (MAX7219_GRAPH) */
/******************************************************************************
 *
 *  Clear all displays
 */

void m7219_clear(void)
{
    m7219_curs(0, 0);
    do
	m7219_putc(' ');
    while (led_col | led_row);	/* fixme: when opt_wrap is off */
}

/******************************************************************************
 *
 *  Set LED intensity (and clear test mode)
 *  in:  0 (minimum) to 15 (maximum)
 */

void m7219_bright(char intensity)
{
    emit_all(0x0a00 | intensity);
}

/******************************************************************************
 *
 *  Send command to all controllers
 */

static void emit_all(int w)
{
    int		i;

    for (i = 0; i < led_count; i++)
	emit_part(w);
    emit_load();
}

/******************************************************************************
 *
 *  Send word to controller(s)
 */

static void emit_word(int w)
{
    emit_part(w);
    emit_load();
}

/******************************************************************************
 *
 *  Send one word as part of a chain
 */

static void emit_part(int w)
{
    w;
__asm
    ldw		x, (3, sp)
    push	#16
00001$:
    rlcw	x
    bccm	_PORT_ODR, #PIN_DATA
    dec		(1, sp)
    bset	_PORT_ODR, #PIN_CLK
    bres	_PORT_ODR, #PIN_CLK
    jrne	00001$
    pop		a
__endasm;
}

/******************************************************************************
 *
 *  Send load signal
 */

static void emit_load(void)
{
__asm
    bset	_PORT_ODR, #PIN_LOAD
    bres	_PORT_ODR, #PIN_LOAD
__endasm;
}

/******************************************************************************
 *
 *  Set LED options
 */

void m7219_option(char opt)
{
    switch (opt) {
    case MAX7219_WRAP :
	opt_wrap = 1;
	break;
    case MAX7219_NOWRAP :
	opt_wrap = 0;
	break;
    case MAX7219_MARQUEE :
	opt_marquee = 1;
	break;
    }

}

/******************************************************************************
 *
 *	 aaa		 666
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
    0x30,		/* I shared with 1 */
    0x38,		/* J */
    SEG_INVALID,	/* K */
    0x0e,		/* L */
    SEG_INVALID,	/* M */
    0x15,		/* N (small n) */
    0x1d,		/* O (small o) */
    0x67,		/* P */
    SEG_INVALID,	/* Q */
    0x05,		/* R (small r) */
    0x5b,		/* S shared with 5 */
    0x0f,		/* T (small t) */
    0x3e		/* U */
};
