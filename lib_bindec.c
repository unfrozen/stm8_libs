/*
 *  File name:  lib_bindec.c
 *  Date first: 12/22/2017
 *  Date last:  12/24/2017
 *
 *  Description: Library of binary/decimal functions for STM8
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  SDCC and COSMIC have different calling conventions.
 *  Define SDCC here or in the Makefile, COSMIC defaults otherwise.
 */

#ifndef SDCC
#define COSMIC
#endif

char *junk_ptr;
short junk_short;
long  junk_long;

/******************************************************************************
 *
 *  Binary to decimal
 *
 *  in:  binary (16 bits), 5 char buffer
 */

void bin16_dec(short bin, char *dec)
{
    bin, dec;

#ifdef SDCC
__asm
    ldw		x, (3, sp)
#endif
#ifdef COSMIC
#asm		/* binary provided in X */
#endif
    sub		sp, #4

    ldw		y, #10000
    divw	x, y
    ld		a, xl
    add		a, #'0'
    ld		(1, sp), a

    ldw		x, #1000
    exgw       	x, y
    divw	x, y
    ld		a, xl
    add		a, #'0'
    ld		(2, sp), a

    ldw		x, #100
    exgw	x, y
    divw	x, y
    ld		a, xl
    add		a, #'0'
    ld		(3, sp), a

    ldw		x, #10
    exgw	x, y
    divw	x, y
    ld		a, xl
    add		a, #'0'
    ld		(4, sp), a

    ld		a, yl
    add		a, #'0'
#ifdef SDCC
    ldw		x, (9, sp)	/* decimal buffer */
#endif
#ifdef COSMIC
    ldw		x, (10, sp)
#endif
    ld		(4, x), a
    ldw		y, (1, sp)
    ldw		(0, x), y
    ldw		y, (3, sp)
    ldw		(2, x), y

    add		sp, #4
#ifdef SDCC
__endasm;
#endif
#ifdef COSMIC
#endasm
#endif
}

/******************************************************************************
 *
 *  Binary to decimal
 *
 *  in:  binary (32 bits), 10 char buffer
 */

void bin32_dec(long bin, char *dec)
{
    bin, dec;

#ifdef SDCC
__asm
    ldw		x, (7, sp)	; decimal buffer pointer
    ldw		y, sp
    addw	y, #3		; 32 bit binary on stack

    clr	(x)			; zero the BCD area
    clr	(1, x)
    clr	(2, x)
    clr	(3, x)
    clr      	(4, x)

    sub		sp, #2
    ld		a, #32
    ld		(1, sp), a

00001$:
    ld		a, #5
    ld     	(2, sp), a

00002$:
    ld		a, #0x33	; assume DAA is needed
    add		a, (x)
    jrmi	00003$		; N set if nybble was 5-9
    sub		a, #0x30	; nybble was 0-4, so undo DAA
00003$:
    add		a, #8		; push bit-3 into HC
    jrh		00004$
    sub		a, #3		; undo DAA
00004$:
    sub		a, #8		; undo add #8 above
    ld		(x), a

    incw	x
    dec		(2, sp)
    jrne	00002$
    subw	x, #5

    rlc		(3, y)		; rotate 32 bit binary into BCD
    rlc		(2, y)
    rlc		(1, y)
    rlc		(y)
    rlc		(4, x)
    rlc		(3, x)
    rlc		(2, x)
    rlc		(1, x)
    rlc		(x)

    dec		(1, sp)
    jrne	00001$

    ldw		y, x
    addw	y, #9
    addw	x, #4

    ld		a, #5
    ld		(1, sp), a
00005$:
    ld		a, (x)
    and		a, #0x0f
    add		a, #'0'
    ld		(y), a
    decw	y

    ld		a, (x)
    swap	a
    and		a, #0x0f
    add		a, #'0'
    ld		(y), a
    decw	y
    decw	x

    dec		(1, sp)
    jrne	00005$

    add		sp, #2

__endasm;
#endif		/* SDCC */

#ifdef COSMIC
#asm
    ldw		x, (8, sp)	; decimal buffer pointer
    ldw		y, sp
    addw	y, #4		; 32 bit binary on stack

    clr		(x)		; zero the BCD area
    clr		(1, x)
    clr		(2, x)
    clr		(3, x)
    clr      	(4, x)

    sub		sp, #2
    ld		a, #32
    ld		(1, sp), a

bit_loop:
    ld		a, #5
    ld     	(2, sp), a

bcd_loop:
    ld		a, #0x33	; assume DAA is needed
    add		a, (x)
    jrmi	daa_h		; N set if nybble was 5-9
    sub		a, #0x30	; nybble was 0-4, so undo DAA
daa_h:
    add		a, #8		; push bit-3 into HC
    jrh		daa_l
    sub		a, #3		; undo DAA
daa_l:
    sub		a, #8		; undo add #8 above
    ld		(x), a

    incw	x
    dec		(2, sp)
    jrne	bcd_loop
    subw	x, #5

    rlc		(3, y)		; rotate 32 bit binary into BCD
    rlc		(2, y)
    rlc		(1, y)
    rlc		(y)
    rlc		(4, x)
    rlc		(3, x)
    rlc		(2, x)
    rlc		(1, x)
    rlc		(x)

    dec		(1, sp)
    jrne       	bit_loop

    ldw		y, x
    addw	y, #9
    addw	x, #4

    ld		a, #5
    ld		(1, sp), a
ascii_loop:
    ld		a, (x)
    and		a, #0x0f
    add		a, #'0'
    ld		(y), a
    decw	y

    ld		a, (x)
    swap	a
    and		a, #0x0f
    add		a, #'0'
    ld		(y), a
    decw	y
    decw	x

    dec		(1, sp)
    jrne	ascii_loop

    add		sp, #2
#endasm
#endif		/* COSMIC */
}

/******************************************************************************
 *
 *  Trim leading zeroes from decimal buffer
 *  in:  buffer, maximum zeroes to remove
 *  out: first non-zero digit
 */

char *decimal_rlz(char *buf, char max)
{
    while (max) {
	if (*buf != '0')
	    break;
	*buf = ' ';
	buf++;
	max--;
    }
    return buf;
}
