/*
 *  File name:  lib_max6675.c
 *  Date first: 12/12/2022
 *  Date last:  12/12/2022
 *
 *  Description: STM8 Library for MAX6675 thermocouple converter.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2022 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Includes
 */

#include "stm8s_header.h"
#include "lib_max6675.h"
#include <stdint.h>

/******************************************************************************
 *
 * This chip has a very simple SPI interface, and a basic CS*, clock & data,
 * so a basic read function will be used.
 *
 * D3: SPI clock (out)
 * A1: SPI in (MISO)
 * A2: CS* (slow pin)
 */

static uint16_t read16(void);

/******************************************************************************
 *
 *  Initialize device
 */

void max6675_init(void)
{
    PA_DDR |= 0x04;		/* A2 is output. */
    PA_ODR &= 0x04;		/* A2 *CS is active low. */
    PA_CR1 |= 0x06;		/* A2 is push-pull, A1 has pullup. */

    PD_DDR |= 0x08;		/* D3 is output. */
    PD_CR1  = 0x08;		/* D3 push-pull. */
    PD_CR2 |= 0x08;		/* D3 is high speed. */
}

/******************************************************************************
 *
 *  Get current thermocouple temperature.
 *  out: temperature in 0.25 degrees C (0 to < 1024) or MAX6675_ERROR
 */

int16_t max6675_read(void)
{
    uint16_t	result;

    result = read16();
    if (result & 0x04)	/* Thermocouple not connected. */
	return MAX6675_ERROR;
    result >>= 3;
    return result;
}

/* MAX6675 timing:
 *
 * 4.3mhz max clock frequency.
 * 100nS  min clock high, clock low.
 * 100nS  min *CS to clock rise.
 * 100nS  min clock fall to data valid.
 *
 * Note: First data bit (D15) is always low and appears on fall of CS*.
 *
 ******************************************************************************
 *
 *  Read 16-bit result
 *  out: encoded result (0ttttttttttttE0?), t=temp bit, E=error
 */

static uint16_t read16(void)
#pragma disable_warning 59
{
__asm
    bres	_PA_ODR, #2	/* CS* low */
    push	#16
00001$:
    btjt	_PA_IDR, #1, 00002$	/* Get bit into carry. */
00002$:
    rlcw	x
    bset	_PD_ODR, #3	/* Clock high. */
    nop				/* To meet 100nS clock high time. */
    bres	_PD_ODR, #3	/* Clock low. */
    dec		(1, sp)
    jrne	00001$
    add		sp, #1
    bset	_PA_ODR, #2	/* CS* high */
__endasm;
}
