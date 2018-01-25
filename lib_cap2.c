/*
 *  File name:  lib_cap2.c
 *  Date first: 01/08/2018
 *  Date last:  01/09/2018
 *
 *  Description: STM8 Library for capture on Timer2
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Pinout:
 *
 *  Pin 1, PD4: Timer2 channel 1
 */

#include "stm8.h"
#include "lib_cap2.h"

/******************************************************************************
 *
 *  Capture memory
 */

int		cap2_buf[16];
int		cap2_last;
int		cap2_ovfl;
volatile char	cap2_ptr_get;
volatile char	cap2_ptr_put;

/******************************************************************************
 *
 *  Set up timer2 for capture
 */

void cap2_init(char prescale)
{
    cap2_ptr_get = 0;
    cap2_ptr_put = 0;
    cap2_ovfl = 0;
    cap2_last = 0;

    TIM2_IER  = TIMx_CC1IE;	/* interrupt on input 1 compare */
    TIM2_EGR  = TIMx_CC1G;	/* capture on input 1 */
    TIM2_CCMR1 = 1;		/* no filter, no PS, IC1 mapped on TI1FP1 */
    TIM2_CCER1 = TIMx_CC1P | TIMx_CC1E;	/* capture on falling edge */
    TIM2_PSCR  = prescale;	/* prescaler = 2^N */

    TIM2_CR1  = TIMx_CEN;	/* enable counter, don't reload */
}

/******************************************************************************
 *
 *  Get number of captures waiting
 */

char cap2_count(void)
{
    char	count;

    count = cap2_ptr_put - cap2_ptr_get;
    if (count & 0x80)		/* pointer wrap */
	count += 16;
    return count;
}

/******************************************************************************
 *
 *  Get next capture count (wait if needed)
 */

int cap2_get(void)
{
    int		val;

    while (cap2_ptr_get == cap2_ptr_put);

    val = cap2_buf[cap2_ptr_get];
    cap2_ptr_get++;
    cap2_ptr_get &= 0x0f;

    return val;
}

/******************************************************************************
 *
 *  Get overflow count
 */

int cap2_overflow(void)
{
    return cap2_ovfl;
}

/******************************************************************************
 *
 *  Timer2 capture interrupt
 */

void cap2_isr(void) __interrupt (IRQ_TIM2C)
{
    int		cap_cur;
    char	ptr_new;

    TIM2_CCER1 ^= TIMx_CC1P;	/* change edge polarity */

    cap_cur = (TIM2_CCR1H << 8) + TIM2_CCR1L;
    cap2_buf[cap2_ptr_put] = cap_cur - cap2_last;
    cap2_last = cap_cur;

    ptr_new = cap2_ptr_put + 1;
    ptr_new &= 15;
    if (ptr_new == cap2_ptr_get)
	cap2_ovfl++;
    else
	cap2_ptr_put = ptr_new;
}

