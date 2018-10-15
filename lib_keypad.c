/*
 *  File name:  lib_keypad.c
 *  Date first: 10/11/2018
 *  Date last:  10/14/2018
 *
 *  Description: STM8 Library for simple keypad
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"

#include "lib_keypad.h"

static char *kmap, *kcur;

static KP_PIN *cfg_row, *cfg_col;

static char kbuf[KP_BUFSIZE];
static char kget;
static char kput;

static char curkey;		/* current pressed key (or zero) */
static char bounce;		/* debounce counter */
static char poll_count;

static void add_key(char);	/* add key to buffer */
static char check_key(void);	/* check keypad for keypress */
static char check_row(void);	/* check rows for keypress */
static void reset_cols(void);	/* reset column pins */
static void reset_rows(void);

#pragma disable_warning 59

/******************************************************************************
 *
 *  Initialize
 *  in: pin setup info (rows, then columns)
 */

void keypad_init(KP_PIN *rows, KP_PIN *cols)
{
    cfg_row = rows;
    cfg_col = cols;

    kget = 0;
    kput = 0;

    curkey = 0;
    bounce = 0;
    poll_count = KP_POLL_MS;

    reset_cols();
    reset_rows();
}

/******************************************************************************
 *
 *  Poll for keypad status
 */

void keypad_poll(void)
{
    char	key;

    poll_count--;
    if (poll_count)
	return;
    poll_count = KP_POLL_MS;

    key = check_key();
    if (!key) {
	if (!bounce)
	    return;
	bounce--;
	if (bounce)
	    return;
#ifdef KP_KEY_REL
	add_key(curkey | 0x80);	/* notify that key is released */
#endif
	curkey = 0;
	return;
    }
    reset_cols();
    bounce = (KP_DEBOUNCE / KP_POLL_MS);
    if (curkey == key)		/* key still pressed */
	return;
    curkey = key;
    add_key(key);
}

/******************************************************************************
 *
 *  Add key to buffer
 *  in:  key
 */

static void add_key(char key)
{
    kbuf[kput] = key;
    kput++;
    if (kput == KP_BUFSIZE)
	kput = 0;
}

/******************************************************************************
 *
 *  Get key from keypad
 *  out: key or zero
 */

char keypad_getc(void)
{
    char	key;

    if (kget == kput)
	return 0;
    key = kbuf[kget];
    kget++;
    if (kget == KP_BUFSIZE)
	kget = 0;
    return key;
}

/******************************************************************************
 *
 *  Install or change keypad map
 *  in: new keypad map
 */

void keypad_kmap(char *map)
{
    kmap = map;
}

/******************************************************************************
 *
 *  Reset column pins
 *  Set column pins to output and set to 1
 */

static void reset_cols(void)
{
__asm
    ldw		y, _cfg_col
00001$:
    ld		a, (2, y)	; pin mask
    jreq        00090$
    push	a
    ldw		x, y
    addw	y, #3
    ldw		x, (x)		; base of port registers
    or          a, (x)
    ld          (x), a		; ODR set bit
    ld		a, (1, sp)
    or          a, (2, x)	; DDR output
    ld          (2, x), a
    pop         a
    or		a, (3, x)
    ld          (3, x), a	; CR1 not open drain
    jrt         00001$
00090$:
__endasm;
}

/******************************************************************************
 *
 *  Reset row pins
 *  Set row pins to internal pull-up (leave as inputs)
 */

static void reset_rows(void)
{
__asm
    ldw		y, _cfg_row
00001$:
    ld		a, (2, y)	; pin mask
    jreq        00090$
    ldw		x, y
    addw	y, #3
    ldw		x, (x)		; base of port registers
    or		a, (3, x)
    ld          (3, x), a	; CR1 set internal pullup
    jrt         00001$
00090$:
__endasm;
}

/******************************************************************************
 *
 *  Check row for pressed key
 *  in:  (kcur points to next key mapping)
 *  out: key or zero (zero flag valid)
 */

static char check_row(void)
{
__asm
    ldw		y, _cfg_row
00001$:
    ld		a, (2, y)	; pin mask
    jreq        00090$
    ldw		x, y
    addw	y, #3
    ldw		x, (x)		; base of port registers
    and		a, (1, x)	; IDR
    jreq        00002$		; bit clear, key pressed
    ldw		x, _kcur
    incw	x
    ldw		_kcur, x
    jrt         00001$
00002$:
    ldw		x, _kcur
    ld		a, (x)
00090$:
__endasm;
}

/******************************************************************************
 *
 *  Check keypad for pressed key
 *  out: key or zero (zero flag valid)
 */

static char check_key(void)
{
    kcur = kmap;
__asm
    ldw		x, _cfg_col
    pushw	x
00001$:
    ldw		x, (1, sp)
    ld		a, (2, x)
    jreq	00090$
    ldw		x, (x)
    cpl		a
    and		a, (x)
    ld		(x), a
    call	_check_row
    jrne	000090$

    ldw		x, (1, sp)
    addw	x, #3
    ldw		(1, sp), x
    subw	x, #3
    ld		a, (2, x)
    ldw		x, (x)
    or		a, (x)
    ld		(x), a
    jrt		00001$

000090$:
    addw	sp, #2
__endasm;
}

/******************************************************************************
 *
 * This library scans the columns from left to right. Columns are outputs.
 *
 * The rows are input pins, set to use the internal pullup, so the scanning
 * column is low and the others are kept high.
 *
 * Only one key may be pressed at a time.
 */
