/*
 *  File name:  lib_w1209.c
 *  Date first: 01/22/2019
 *  Date last:  02/24/2019
 *
 *  Description: STM8 Library for W1209 thermostat board.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2019 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *  Some common code borrowed from lib_tm1638.c
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"

#include "lib_adc.h"
#include "lib_w1209.h"

static char key_buf[KB_SIZE];
static char key_db[3];		/* current debounce counts */
static volatile char key_get;
static volatile char key_put;

static char display[3];
static char cursor;
static char led_cur;
static char led_update;

static char blink_rate;
static char blink_count;
static char blink_flag;
static char blink_ms;

#define	SEG_INVALID	0x49	/* "invalid", three horizontal bars */

const char segs_w12_digits[];
const char segs_w12_alpha[];

static void poll_blink();
static void poll_keys();
static void poll_leds();

static char code_7seg(char);

/******************************************************************************
 *
 *  Initialize board
 */

void w12_init(void)
{
    CLK_CKDIVR = 0x00;	/* internal clock 16mhz */

    key_get = 0;
    key_put = 0;

    cursor  = 0;
    led_cur = 2;
    led_update = LED_UPDATE;

    PA_DDR = 0x0e;	/* A1-A3 outputs */
    PA_CR1 = 0xff;     	/* inputs have pullup, outputs not open drain */
    PA_CR2 = 0x00;	/* no interrupts on inputs, 2mhz outputs */

    PB_DDR = 0x30;	/* B4-B5 outputs */
    PB_CR1 = 0xff;	/* optional because B4, B5 are open drain */
    PB_CR2 = 0x00;

    PC_DDR = 0xc0;	/* C3-C5 inputs, C6-C7 outputs */
    PC_CR1 = 0xff;
    PC_CR2 = 0x00;

    PD_DDR = 0x3e;	/* D6 input, D1-D5 outputs */
    PD_CR1 = 0xff;
    PD_CR2 = 0x00;

    adc_init(ADC_CH6);	/* D6, pin #3 */

    __asm__ ("rim");
}

/******************************************************************************
 *
 *  Write digit/char to LED
 *  in: ASCII digit or char (use bit-7 for decimal point)
 */

void w12_putc(char c)
{
    char	segs;

    segs = code_7seg(c & 0x7f);
    segs |= (c & 0x80);
    display[cursor] = segs;
    cursor++;
    if (cursor == 3)
	cursor = 0;
}

/******************************************************************************
 *
 *  Write string to LED
 *  in: ASCII string (insert decimal point where desired.)
 */

void w12_puts(char *str)
{
    char	c;
    while (c = *str++) {
	if (*str == '.') {
	    c |= 0x80;
	    str++;
	}
	w12_putc(c);
    }
}

/******************************************************************************
 *
 *  Set cursor position
 *  in: column (0-2)
 */

void w12_curs(char curs)
{
    cursor = curs;
}

/******************************************************************************
 *
 *  Set LED blinking
 *  in: Blink rate * 1/100 second or 0 (disable)
 */
void w12_blink(char rate)
{
    blink_rate = rate;
    blink_count = rate;
    blink_ms = 10;
    if (rate)
        return;
    blink_flag = 0;	/* blink off, back to normal */
}

/******************************************************************************
 *
 *  Get keypress
 *  out: zero or ASCII key ('0' to '2'), bit-7 set if released
 */

char w12_getc(void)
{
    char	key;

    if (key_get == key_put)
	return 0;
    key = key_buf[key_get];
    key_get++;
    if (key_get == KB_SIZE)
	key_get = 0;
    return key;
}

/******************************************************************************
 *
 *  Poll for display, keys, and blinking
 *  (call every millisecond)
 */
void w12_poll(void)
{
    led_update--;
    if (!led_update) {
	led_update = LED_UPDATE;
	poll_leds();
    }
    poll_keys();
    poll_blink();
}

/******************************************************************************
 *
 *  Turn the relay and LED on/off
 *  in: zero = off, non-zero = on
 */

void w12_relay(char on)
{
    on;
__asm
    ld		a, (3, sp)
    sub		a, #1		; carry if zero
    ccf				; carry if non-zero
    bccm	_PA_ODR, #3
__endasm;
}

/******************************************************************************
 *
 *  Get the current temperature probe value
 *  out: current A/D value, -1 if timeout
 */

short w12_probe(void)
{
    return adc_avg(6);
}

/******************************************************************************
 *
 *  Poll for blink event
 *  (Call every millisecond)
 */

static void poll_blink(void)
{
    if (!blink_rate)
	return;
    blink_ms--;
    if (blink_ms)
	return;
    blink_ms = 10;
    blink_count--;
    if (blink_count)
	return;
    blink_count = blink_rate;
    blink_flag ^= 1;		/* toggle LEDs */
}

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
 *  Poll for keypress
 *  (Call every millisecond)
 */

static void poll_keys(void)
{
    char	i, *db;
    char	key_status;

    key_status = PC_IDR >> 3;	/* keys are C3, C4, C5 */
    key_status ^= 0xff;		/* active low */

    db = key_db;
    for (i = 0; i < 3; i++) {
	if (*db) {
	    (*db)--;
	    if (*db == 0)	/* key released */
		key_add(i + '0' + 0x80);
	}
	if (key_status & 1) {
	    if (*db == 0)
		key_add(i + '0');
	    *db = KB_DEBOUNCE;
	}
	db++;
	key_status >>= 1;
    }

}

/******************************************************************************
 *
 *  Poll for LED display
 *  (Call every millisecond)
 */

static void poll_leds(void)
{
__asm
    bset	_PD_ODR, #4	; LED #1
    bset	_PB_ODR, #5	; LED #2
    bset	_PB_ODR, #4	; LED #3

    tnz		_blink_flag
    jrne	00090$

    ld		a, _led_cur
    clrw	x
    ld		xl, a
    ld		a, (_display, x)

    rrc		a
    bccm	_PD_ODR, #5	; A
    rrc		a
    bccm	_PA_ODR, #2	; B
    rrc		a
    bccm	_PC_ODR, #7	; C
    rrc		a
    bccm	_PD_ODR, #3	; D
    rrc		a
    bccm	_PD_ODR, #1	; E
    rrc		a
    bccm	_PA_ODR, #1	; F
    rrc		a
    bccm	_PC_ODR, #6	; G
    rrc		a
    bccm	_PD_ODR, #2	; DP

    dec		_led_cur
    jrmi	00001$		; LED #1
    jreq	00002$		; LED #2

    bres	_PB_ODR, #4	; LED #3
    jra		00090$

00001$:
    bres	_PD_ODR, #4
    mov		_led_cur, #2
    jra		00090$

00002$:
    bres	_PB_ODR, #5

00090$:
__endasm;
}

/******************************************************************************
 *
 *  Get 7-segment code for ASCII character
 */

static char code_7seg(char c)
{
    if ((c & 0xf0) == 0x30)
	return segs_w12_digits[c & 0x0f];
    if (c >= 'A' &&
	c <= 'U')
	return segs_w12_alpha[c - 'A'];

    if (c == ' ')
	return 0;
    if (c == '-')
	return 0x40;
    if (c == '_')
	return 8;
    if (c == '[')
	return 0x39;
    if (c == ']')
	return 0x0f;

    return SEG_INVALID;
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

const char segs_w12_digits[16] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,	/* 0 - 4 */
    0x6d, 0x7d, 0x07, 0x7f, 0x67,	/* 5 - 9 */
    SEG_INVALID, SEG_INVALID, SEG_INVALID, 0x48, SEG_INVALID, SEG_INVALID
};
const char segs_w12_alpha[21] = {
    0x77,		/* A */
    0x7c,		/* B */
    0x39,		/* C */
    0x5e,		/* D */
    0x79,		/* E */
    0x71,		/* F */
    SEG_INVALID,	/* G */
    0x76,		/* H */
    0x30,		/* I (left side) */
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
    0x78,		/* T */
    0x3e		/* U */
};

/******************************************************************************
 *
 *  Board layout:
 *  Source: https://github.com/TG9541/stm8ef/wiki/Board-W1209

1	PD4	7S-Dig1 (200R)
2	PD5	7S-A
3	PD6	Rx,AIN6 ((5.0V-20k)-AIN6-(NTC10k||470nF)-GND)
4	NRST	J2.3
5	PA1	7S-B
6	PA2	7S-F
7	VSS	GND
8	Vcap	C 
9	VDD	+5.0V
10	PA3	Relay (1:on) 2k/10k-NPN,Relay,LED
11	PB5	7S-Dig2 (200R)
12	PB4	7S-Dig3 (200R)
13	PC3	Key "set"
14	PC4	Key "+"
15	PC5	Key "-"
16	PC6	7S-G
17	PC7	7S-C
18	PD1	7S-E / SWIM J.2
19	PD2	7S-DP
20	PD3	7S-D

LED:

  D4	LED #1 anode
  B5	LED #2 anode
  B4	LED #3 anode

  A	D5
  B	A1
  C	C7
  D	D3
  E	D1
  F	A2
  G	C6
  DP	D2

KEYS:

  SET	C3
  +	C4
  -	C5

RELAY:	A3



*/
