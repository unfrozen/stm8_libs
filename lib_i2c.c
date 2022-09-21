/*
 *  File name:  lib_i2c.c
 *  Date first: 05/17/2018
 *  Date last:  01/29/2019
 *
 *  Description: Library for communicating with I2C devices.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018, 2019 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */
#include "stm8s_header.h"
#include "lib_i2c.h"

/*
 *  I2C pins, PD2=clock, PD3=data
 *  (On STM8S103 board, D3 is next to 5v, for easy pull-up resistor)
 */

#define I2C_CLOCK_DDR	_PD_DDR
#define I2C_CLOCK_ODR	_PD_ODR
#define I2C_CLOCK_CR1	_PD_CR1
#define I2C_CLOCK_CR2	_PD_CR2
#define I2C_CLOCK_PIN	2

#define I2C_DATA_DDR	_PD_DDR
#define I2C_DATA_ODR	_PD_ODR
#define I2C_DATA_IDR	_PD_IDR
#define I2C_DATA_CR1	_PD_CR1
#define I2C_DATA_CR2	_PD_CR2
#define I2C_DATA_PIN	3

static void i2c_rxbit(void);

#warning "I2C clock and data have been moved from D1 and D2 to D2 and D3."
#warning "Please change code or connections and remove this warning."

/******************************************************************************
 *
 *  Initialize
 */

void i2c_init(void)
{
__asm
    bset	I2C_CLOCK_DDR, # I2C_CLOCK_PIN
    bset	I2C_CLOCK_CR1, # I2C_CLOCK_PIN	; no open drain

    bres	I2C_DATA_DDR, # I2C_DATA_PIN
    bres	I2C_DATA_ODR, # I2C_DATA_PIN	; zero when output
    bset	I2C_DATA_CR1, # I2C_DATA_PIN	; pullup when input
__endasm;

    i2c_clock0();		/* prevent start or stop codes */
    i2c_data1();		/* safely raise data */
    i2c_clock1();		/* now clock=data=1 */


}

/******************************************************************************
 *
 *  Transmit 8 bits
 */

void i2c_txbit8(char bits)
{
    bits;
__asm
    push	#8
00001$:
#if __SDCCCALL == 0
    sll		(4, sp)
#else
    sll		a
#endif
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

/******************************************************************************
 *
 *  Receive 8 bits
 */
#pragma disable_warning 59

char i2c_rxbit8(void)
{
__asm
    push	#8
00001$:
    call	_i2c_rxbit
    rlc		a
    dec		(1, sp)
    jrne	00001$

    add		sp, #1
__endasm;
}

/******************************************************************************
 *
 *  Receive 1 bit
 *  out: bit returned in carry flag
 */

static void i2c_rxbit(void)
{
__asm
    call	_i2c_clock1
    btjt	I2C_DATA_IDR, #I2C_DATA_PIN, 00001$
00001$:
    call	_i2c_clock0
__endasm;
}

/******************************************************************************
 *
 *  Get ACK from slave
 *  out: zero = timeout
 */

char i2c_getack(void)
{
__asm
    call	_i2c_data1
    call	_i2c_clock1
    clr		a
00001$:
    dec		a
    jreq	00090$
    btjt	I2C_DATA_IDR, #I2C_DATA_PIN, 00001$
000090$:
    call	_i2c_clock0
__endasm;
}

/******************************************************************************
 *
 *  Send ACK or NAK to slave
 */

void i2c_sendack(void)
{
    i2c_data0();		/* ACK */
    i2c_clock1();
    i2c_clock0();
    i2c_data1();
}
void i2c_sendnak(void)
{
    i2c_data1();		/* NAK */
    i2c_clock1();
    i2c_clock0();
    i2c_data1();
}

/******************************************************************************
 *
 *  I2C Start and Stop
 *  Start: CLK=DAT=1 -> CLK=DAT=0
 *  Stop:  CLK=0     -> CLK=DAT=1
 */

void i2c_start(void)
{
    i2c_data0();
    i2c_data0();
    i2c_clock0();
}

void i2c_stop(void)
{
    i2c_data0();
    i2c_clock1();
    i2c_clock1();
    i2c_data1();
}

/******************************************************************************
 *
 *  Clock and data pins
 *  call + bit + return = 9 cycles
 *  no regs or flags modified
 */

void i2c_clock0(void)
{
__asm
    bres	I2C_CLOCK_ODR, #I2C_CLOCK_PIN
__endasm;
}

void i2c_clock1(void)
{
__asm
    bset	I2C_CLOCK_ODR, #I2C_CLOCK_PIN
__endasm;
}

void i2c_data0(void)
{
__asm
    bset	I2C_DATA_DDR, #I2C_DATA_PIN
__endasm;
}

void i2c_data1(void)
{
__asm
    bres	I2C_DATA_DDR, #I2C_DATA_PIN
__endasm;
}

