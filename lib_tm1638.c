/*
 *  File name:  lib_tm1638.c
 *  Date first: 06/08/2018
 *  Date last:  06/24/2018
 *
 *  Description: STM8 Library for TM1638 LED array and keys.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Some code copied from lib_max7219.c
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"
#include "lib_tm1638.h"

#define PORT_ODR PD_ODR
#define PORT_DDR PD_DDR
#define PORT_CR1 PD_CR1
#define _PORT_ODR _PD_ODR
#define _PORT_DDR _PD_DDR
#define _PORT_IDR _PD_IDR

#define PIN_CLK	 1		/* D1 is module CLK */
#define PIN_STB  2		/* D2 is module CS/STROBE */
#define PIN_DATA 3		/* D3 is module DIN */
#define PIN_MASK ((1 << PIN_CLK) | (1 << PIN_STB) | (1 << PIN_DATA))

#define	SEG_INVALID	0x49	/* "invalid", three horizontal bars */

#define KB_SIZE		10	/* size of key buffer */
#define KB_DEBOUNCE	40	/* debounce count in milliseconds */

static char led_type;		/* either 8 key (with 8 LEDS) or 16 key */
static char led_col;		/* digit 7-seg always 0-7 */
static char led_bright;		/* normal brightness */

static char blink_rate;		/* blink rate, time 1/100 second */
static char blink_count;
static char blink_ms;
static char blink_flags;

static void emit_byte(char);	/* send byte */
static char read_byte(void);	/* read byte */

static char  code_7seg(char);	/* get 7-segment code for ASCII character */

static void key_read(void);	/* read keys and build key_status */
static void key_map8(char *);
static void key_map16(char *);

const char segs_1638_digits[];
const char segs_1638_alpha[];

const  char  kb_default[] = "0123456789ABCDEF";
static char *kb_current;

static int  key_status;		/* current state of all 8 or 16 keys */
static char key_count;		/* number of keys, 8 or 16 */
static char key_db[16];		/* current debounce count for all keys */
static char key_buf[KB_SIZE];
static char key_get;
static char key_put;

#ifdef TM1638_16_KEYS
static char leds[8];		/* storage for assembling the LED segments */
static char led_bit;		/* bit position for current LED */
static void led_segs(char);	/* store new segments in array */
#endif

static volatile char lock;

/******************************************************************************
 *
 *  Initialize device
 *  in: type
 */

void tm1638_init(char type)
{
    char	i;

    PORT_ODR |= PIN_MASK;	/* start pins high */
    PORT_DDR |= PIN_MASK;	/* D1, D2, D3 outputs */
    PORT_CR1 |= PIN_MASK;	/* enable source output */

    led_type  = type;
    led_col = 0;

    tm1638_bright(5);		/* active and brightness */

    tm1638_clear();

    key_get = 0;
    key_put = 0;
    kb_current = (char *)kb_default;
    lock = 0;
    blink_rate = 0;
    key_count = 8;
#ifdef TM1638_16_KEYS
    led_bit = 0x80;
    if (led_type == TM1638_16)
	key_count = 16;
#endif
    for (i = 0; i < key_count; i++)
	key_db[0] = 0;
}

/******************************************************************************
 *
 *  Set cursor position
 *  in: line or unit# (0-max), column
 */

void tm1638_curs(char col)
{
    led_col = col;
#ifdef TM1638_16_KEYS
    if (led_type == TM1638_8)
	return;
    led_bit = 0x80 >> col;
#endif
}

/******************************************************************************
 *
 *  Write number/string to unit
 *  in: number/string
 */

void tm1638_puts(char *str)
{
    char	c;
    while (c = *str++) {
	if (*str == '.') {
	    c |= 0x80;
	    str++;
	}
	tm1638_putc(c);
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
	return 0x40;
    if (c == '_')
	return 8;
    if ((c & 0xf0) == 0x30)
	return segs_1638_digits[c & 0x0f];
    if (c < 'A' ||
	c > 'U')
	return SEG_INVALID;
    return segs_1638_alpha[c - 'A'];
}

/******************************************************************************
 *
 *  Raise and lower STROBE (enable) pin
 */

inline void strobe_hi(void)
{
__asm
    bset	_PORT_ODR, #PIN_STB
__endasm;
}
inline void strobe_lo(void)
{
__asm
    bres	_PORT_ODR, #PIN_STB
__endasm;
}

/******************************************************************************
 *
 *  Write digit/char to unit
 *  in: digit or char (bit-7 for decimal point)
 */

void tm1638_putc(char c)
{
    char	pos, segs;

    segs = code_7seg(c & 0x7f);
    segs |= (c & 0x80);

#ifdef TM1638_16_KEYS
    if (led_type == TM1638_16) {
	led_segs(segs);
	return;
	/* we don't update led_col because led_bit is used instead */
    }
#endif
    pos = led_col << 1;

    lock = 1;
    strobe_lo();
    emit_byte(0x44);		/* data write, no increment */
    strobe_hi();
    strobe_lo();
    emit_byte(0xc0 | pos);
    emit_byte(segs);
    strobe_hi();
    lock = 0;

    led_col++;
    led_col &= 7;
}

/******************************************************************************
 *
 *  Set LED status (8 key board only)
 *  in:  LED number (0 to 7), new status (0=off, 1=red)
 */
void tm1638_setled(char led, char val)
{
    led <<= 1;
    led++;

    lock = 1;
    strobe_lo();
    emit_byte(0x44);
    strobe_hi();
    strobe_lo();
    emit_byte(0xc0 | led);
    emit_byte(val);
    strobe_hi();
    lock = 0;
}

/******************************************************************************
 *
 *  Clear all displays
 */

void tm1638_clear(void)
{
    tm1638_curs(0);
    do
	tm1638_putc(' ');
    while (led_col);
}

/******************************************************************************
 *
 *  Set LED intensity (and clear test mode)
 *  in:  0 (minimum) to 7 (maximum)
 */

void tm1638_bright(char intensity)
{
    lock = 1;
    led_bright = intensity;
    strobe_lo();
    emit_byte(0x88 | intensity);
    strobe_hi();
    lock = 0;
}

/******************************************************************************
*
 *  Set LED blinking
 *  Alternates between regular and minimum brightness.
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void tm1638_blink(char rate)
{
    blink_rate = rate;
    blink_count = rate;
    blink_flags = 0x80;
    blink_ms = 10;
    if (rate)
	return;
    tm1638_bright(led_bright);	/* blink off, back to normal */
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
	strobe_lo();
	emit_byte(command);
	strobe_hi();
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
 *  Send one byte
 */

static void emit_byte(char b)
{
    b;
__asm
    ld		a, (3, sp)
    push	#8
00001$:
    rrc		a
    bccm	_PORT_ODR, #PIN_DATA
    bres	_PORT_ODR, #PIN_CLK
    dec		(1, sp)
    bset	_PORT_ODR, #PIN_CLK
    jrne	00001$
    pop		a
__endasm;
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

const char segs_1638_digits[16] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,	/* 0 - 4 */
    0x6d, 0x7d, 0x07, 0x7f, 0x67,	/* 5 - 9 */
    SEG_INVALID, SEG_INVALID, SEG_INVALID, 0x48, SEG_INVALID, SEG_INVALID
};
const char segs_1638_alpha[21] = {
    0x77,		/* A */
    0x7c,		/* B */
    0x39,		/* C */
    0x5e,		/* D */
    0x79,		/* E */
    0x71,		/* F */
    SEG_INVALID,	/* G */
    0x76,		/* H */
    SEG_INVALID,	/* I */
    0x0e,		/* J */
    SEG_INVALID,	/* K */
    0x38,		/* L */
    SEG_INVALID,	/* M */
    SEG_INVALID,	/* N */
    0x5c,		/* O (small o) */
    0x73,		/* P */
    SEG_INVALID,	/* Q */
    SEG_INVALID,	/* R */
    0x6d,		/* S shared with 5 */
    SEG_INVALID,	/* T */
    0x3e		/* U */
};

#ifdef TM1638_16_KEYS
/******************************************************************************
 *
 *  Store new segments in LED segment map
 *  in:  new segments
 */

static void led_segs(char segs)
{
    char	mask, i;

    mask = ~led_bit;
    for (i = 0; i < 8; i++) {
	leds[i] &= mask;
	if (segs & 1)
	    leds[i] |= led_bit;
	segs >>= 1;
    }
    led_bit >>= 1;
    if (!led_bit)
	led_bit = 0x80;
}

/******************************************************************************
 *
 *  Push LED segment map to LEDs
 */

void tm1638_push(void)
{
    char	i, *ptr;

    lock = 1;
    strobe_lo();
    emit_byte(0x40);		/* data write, auto-increment */
    strobe_hi();

    strobe_lo();
    emit_byte(0xc0);

    ptr = leds;
    for (i = 0; i < 8; i++) {
	emit_byte(*ptr++);
	emit_byte(0);		/* 16-key module does not have row of LEDs */
    }
    strobe_hi();
    lock = 0;
}

#endif	/* TM1638_16_KEYS */

/******************************************************************************
 *
 *  Add key into buffer
 */

static void key_add(char key)
{
    char	put, *ptr;

    put = key_put;
    ptr = key_buf + put;
    put++;
    if (put == KB_SIZE)
	put = 0;
    if (put == key_get)		/* buffer full? */
	return;
    *ptr = key;
    key_put = put;
}

/******************************************************************************
 *
 *  Poll for key press and blinking
 *  (every millisecond)
 */
void tm1638_poll(void)
{
    char	i, *db;

    blink_check();

    if (lock)
	return;
    key_read();			/* read keys and build key_status */

/* Decrement all key debounce counts.
 * If key is pressed:
 *   Set debounce
 *   If debounce was 0, treat as new keypress
 *   If debounce was > 0, ignore
 */
    db = key_db;
    for (i = 0; i < key_count; i++) {
	if (*db) {
	    (*db)--;
	    if (*db == 0)	/* key released */
		key_add(kb_current[i] | 0x80);
	}
	if (key_status & 1) {
	    if (*db == 0)
		key_add(kb_current[i]);
	    *db = KB_DEBOUNCE;
	}
	db++;
	key_status >>= 1;
    }
}

/******************************************************************************
 *
 *  Get key
 *  out: key char ("0123456789ABCDEF"), zero if none
 *  Bit-7 is low for key press, high for release.
 */
char tm1638_getc(void)
{
    char	key, *ptr;

    if (key_get == key_put)
	return 0;
    ptr = key_buf + key_get;
    key = *ptr;
    key_get++;
    if (key_get == KB_SIZE)
	key_get = 0;
    return key;
}

/******************************************************************************
 *
 *  Set alternate key mappings
 *  in:  key map has 8 or 16 characters for keys
 */
void tm1638_kmap(char *map)
{
    kb_current = map;
}

/******************************************************************************
 *
 *  Read keys and build key_status
 */

static void key_read(void)
{
    char	vals[4], i;

    key_status = 0;

    strobe_lo();
    emit_byte(0x42);
    PORT_DDR &= ~(1 << PIN_DATA);
    for (i = 0; i < 4; i++)
	vals[i] = read_byte();
    PORT_DDR |= (1 << PIN_DATA);
    strobe_hi();

    if (led_type == TM1638_8) {
	key_map8(vals);
	return;
    }
#ifdef TM1638_16_KEYS
    if (led_type == TM1638_16)
	key_map16(vals);
#endif
}

/******************************************************************************
 *
 *  Read one byte
 */

static void wait_500(void)
{
}
#pragma disable_warning 59

static char read_byte(void)
{
__asm
    push	#0
    push	#8
00001$:
    bres	_PORT_ODR, #PIN_CLK
    call	_wait_500
    ld		a, _PORT_IDR
    and		a, #(1 << PIN_DATA)
    add		a, #0xff
    rrc		(2, sp)
    bset	_PORT_ODR, #PIN_CLK
    dec		(1, sp)
    jrne	00001$
    pop		a
    pop		a
__endasm;
}

/******************************************************************************
 *
 *  Layout and coding for 8 key module:
 *
 *  Board layout:	xx  xx  xx   5  xx  xx  xx   1  byte #1
 *  1 2 3 4 5 6 7 8	xx  xx  xx   6  xx  xx  xx   2  byte #2
 *			xx  xx  xx   7  xx  xx  xx   3  byte #3
 *			xx  xx  xx   8  xx  xx  xx   4  byte #4
 *
 *  Map 8-key results into key_status
 */

void key_map8(char *vals)
{
    vals;
__asm
    ldw		x, (3, sp)
    ldw		y, #_key_status+1
    srl		(x)
    rrc		(y)
    srl		(1, x)
    rrc		(y)
    srl		(2, x)
    rrc		(y)
    srl		(3, x)
    rrc		(y)

    ld		a, (x)
    add		a, #0xff
    rrc		(y)
    ld		a, (1, x)
    add		a, #0xff
    rrc		(y)
    ld		a, (2, x)
    add		a, #0xff
    rrc		(y)
    ld		a, (3, x)
    add		a, #0xff
    rrc		(y)
__endasm;
}
 
/******************************************************************************
 *
 *  Layout and coding for 16 key module:
 *
 *  Board layout:	b7  b6  b5  b4  b3  b2  b1  b0
 *  1  2  3  4		xx   2  10  xx  xx   1   9  xx  byte #1
 *  5  6  7  8		xx   4  12  xx  xx   3  11  xx  byte #2
 *  9 10 11 12		xx   6  14  xx  xx   5  13  xx  byte #3
 * 13 14 15 16		xx   8  16  xx  xx   7  15  xx  byte #4
 *
 *  Map 16-key results into key_status
 */
#ifdef TM1638_16_KEYS

void key_map16(char *vals)
{
    vals;
__asm
    ldw		x, (3, sp)
    sub		sp, #5

    sll		(x)
    sll		(1, x)
    sll		(2, x)
    sll		(3, x)

    ld		a, (x)
    swap	a
    ld		(1, sp), a
    ld		a, (1, x)
    swap	a
    ld		(2, sp), a
    ld		a, (2, x)
    swap	a
    ld		(3, sp), a
    ld		a, (3, x)
    swap	a
    ld		(4, sp), a

    ld		a, #2
    ld		(5, sp), a
00001$:
    sll		(1, sp)		// 1 & 9
    rrcw	y
    sll		(x)		// 2 & 10
    rrcw	y

    sll		(2, sp)		// 3 & 11
    rrcw	y
    sll		(1, x)		// 4 & 12
    rrcw	y

    sll		(3, sp)		// 5 & 13
    rrcw	y
    sll		(2, x)		// 6 & 14
    rrcw	y

    sll		(4, sp)		// 7 & 15
    rrcw	y
    sll		(3, x)		// 8 & 16
    rrcw	y

    dec		(5, sp)
    jrne	00001$

    ldw		_key_status, y

    add		sp, #5
__endasm;
}
#endif
