/*
 *  File name:  lib_board.c
 *  Date first: 07/01/2019
 *  Date last:  07/01/2019
 *
 *  Description: STM8 Library for popular board support (stm8s103F3, stm8s105).
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2019 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"
#include "lib_board.h"

/******************************************************************************
 *
 *  Initialize board
 *
 *  Board frequency override may be added later.
 *  Default is 16mhz internal for stm8s103 and 8mhz external xtal for stm8s105.
 *  SQU board for CAN uses the 16mhz crystal.
 */

void board_init(char freq)
{
    freq;
#ifdef STM8103
    CLK_CKDIVR = 0x00;	/* clock 16mhz internal for STM8S103 */

    PB_DDR = 0x20;	/* output LED */
    PB_CR1 = 0xff;     	/* inputs have pullup */
    PB_CR2 = 0x00;	/* no interrupts, 2mhz output */

#endif
#ifdef STM8105
    CLK_CKDIVR = 0x00;  /* no clock divisor */
    CLK_ECKR = 1;       /* enable crystal oscillator */
    CLK_SWCR = 2;       /* enable clock switch */
    CLK_SWR = 0xb4;     /* HSE is master (8 mhz crystal) */

    PE_DDR = 0x20;	/* output LED */
    PE_CR1 = 0x20;
#endif
#ifdef STM8S207
    CLK_CKDIVR = 0x00;  /* no clock divisor */
    CLK_ECKR = 1;       /* enable crystal oscillator */
    CLK_SWCR = 2;       /* enable clock switch */
    CLK_SWR = 0xb4;     /* HSE is master (16 mhz crystal) */
#endif
__ASM
    rim
__ENDASM
}

/* Available ports on STM8S103P3:
 *
 * A1..A3	A3 is HS
 * B4..B5	Open drain
 * C3..C7	HS
 * D1..D6	HS
 *
 * Available ports on 105 demo board:
 *
 * A1-A2        slow (crystal) (A3 lost from 103)
 * B0-B5        slow
 * C1-C7        fast all high sink
 * D0-D4        fast and high sink
 * D5-D7        slow (D5/D6 UART)
 * E5           slow LED
 * F4           slow
 *
 ******************************************************************************
 *
 *  Turn LED on or off.
 *  in: zero = off, non-zero = on
 */

void board_led(char on)
{
#ifdef  STM8103
    if (on)
        PB_ODR &= 0xdf;
    else
        PB_ODR |= 0x20;
#endif
#ifdef  STM8105
    if (on)
        PE_ODR &= 0xdf;
    else
        PE_ODR |= 0x20;
#endif
#ifdef STM8S207
    if (on)
	PD_ODR |= 0x80;
    else
	PD_ODR &= 0x7f;

#endif
}

