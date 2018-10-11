/*
 *  File name:  lib_adc.c
 *  Date first: 09/14/2018
 *  Date last:  09/15/2018
 *
 *  Description: Library for using STM8S ADC.
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

#include "lib_adc.h"

#ifdef STM8103
#define CR1_VAL 0x21		/* 16mhz / 4 = 4mhz ADC */
#endif
#ifdef STM8105
#define CR1_VAL	0x01		/* 8mhz / 2 = 4mhz ADC */
#endif

/******************************************************************************
 *
 *  Initialize
 *  in:  bits for ADC channel(s) to use
 *  (Note that this may be called from multiple sub-systems)
 */

void adc_init(char chans)
{
#ifdef ORIG_C
    ADC_TDRL |= chans & ADC_MASK; /* disable schmitt triggers for analog */
    if (chans & ADC_CH12)
	ADC_TDRH |= 0x10;
    ADC_CR2 = 0x08;		/* result is right aligned */
    ADC_CR1 = CR1_VAL;		/* enable ADC */
#else
    chans;
__asm
    ld		a, (3, sp)
    and		a, #ADC_MASK
    ldw		x, #_ADC_TDRL
    or		a, (x)
    ld		(x), a

    ld		a, (3, sp)
    and		a, #ADC_CH12
    jreq       	00001$
    bset	_ADC_TDRH, #4
00001$:
    mov		_ADC_CR2, #0x08
    mov		_ADC_CR1, #CR1_VAL
__endasm;	
#endif
}

/******************************************************************************
 *
 *  Get average of 4 A/D samples
 *  in:  ADC channel (0 to 12)
 *  out: 10-bit A/D average value, -1 if timeout
 */
short adc_avg(char chan)
{
#ifdef ORIG_C
    short	avg, val;
    char	i;

    avg = 2;
    for (i = 0; i < 4; i++) {
	val = adc_val(chan);
	if (val < 0)
	    return -1;
	avg += val;
    }
    return avg >> 2;
#else
#pragma disable_warning 59
    chan;
__asm
    ldw		x, #2
    pushw	x
    push	#4
00001$:
    ld		a, (6, sp)
    push	a
    call	_adc_val
    pop		a
    tnzw	x
    jrmi	000090$
    addw	x, (2, sp)
    ldw		(2, sp), x
    dec		(1, sp)
    jrne	00001$

    srlw	x
    srlw	x
00090$:
    add		sp, #3
__endasm;
#endif
}

/******************************************************************************
 *
 *  Get current raw A/D value
 *  in:  ADC channel (0 to 12)
 *  out: 10-bit A/D conversion value, -1 if timeout
 */
#pragma disable_warning 59

short adc_val(char chan)
{
    chan;
__asm
    bres	_ADC_CSR, #7	; clear end of conversion flag
    ld		a, (3, sp)
    ld		_ADC_CSR, a	; set A/D channel
    bset	_ADC_CR1, #0	; start conversion

    ld		a, #100		; conversion is 3.5 uS, 56 cycles @ 16mhz
00001$:
    btjt	_ADC_CSR, #7, 00090$	; (2)
    dec		a			; (1)
    jrne	00001$			; (2) = 5 per loop, should be done < 12
    clrw	x
    decw	x
    ret
00090$:
    ld		a, _ADC_DRL	; (24.11.8) read DRL first with right-justify
    ld		xl, a
    ld		a, _ADC_DRH
    ld		xh, a
__endasm;
}
