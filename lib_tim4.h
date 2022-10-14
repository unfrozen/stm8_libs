/*
 *  File name:  lib_tim4.h
 *  Date first: 03/23/2018
 *  Date last:  12/02/2020
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
 * Initialize the clock (set up timer 4)
 * in: Millisecond callback, 1/10 second callback
 */

void tim4_init(void (*)(void), void (*)(void));

void tim4_isr(void) __interrupt (IRQ_TIM4);

