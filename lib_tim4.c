/*
 *  File name:  lib_tim4.c
 *  Date first: 03/23/2018
 *  Date last:  12/02/2018
 *
 *  Description: Library for millisecond and 1/10 second callbacks using tim4.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018-2020 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 *  This is a stripped version of lib_clock.
 *
 ******************************************************************************
 *
 */

#include "stm8s_header.h"

#include "lib_tim4.h"

static signed char clock_ms;	/* milliseconds, may be negative with trim */

static void (*timer_ms)(void);
static void (*timer_10)(void);

/******************************************************************************
 *
 * Initialize the clock (set up timer 4)
 * in: Millisecond callback, 1/10 second callback
 */

void tim4_init(void (*call_ms)(void), void (*call_10)(void))
{
    timer_ms = call_ms;
    timer_10 = call_10;

#ifdef STM8103
    TIM4_PSCR = 6;	/* prescaler = 64 for 16mhz */
#endif
#ifdef STM8105
    TIM4_PSCR = 5;	/* prescaler = 32 for 8mhz */
#endif
#ifdef STM8S207
    TIM4_PSCR = 6;	/* prescaler = 64 for 16mhz */
#endif
    TIM4_ARR  = 249;	/* reset and interrupt every 1.0 ms */
    TIM4_CR1  = 1;	/* enable timer4 */
    TIM4_IER  = 1;	/* enable timer4 interrupt */
}

/******************************************************************************
 *
 *  Timer 4 interrupt
 */

void tim4_isr(void) __interrupt (IRQ_TIM4)
{
    TIM4_SR = 0;		/* clear the interrupt */

    timer_ms();
    clock_ms++;
    if (clock_ms < 100)
	return;
    clock_ms -= 100;

    timer_10();
}
