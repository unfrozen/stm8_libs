/*
 *  File name:  lib_clock.c
 *  Date first: 03/23/2018
 *  Date last:  03/27/2018
 *
 *  Description: Library for maintaining a wall clock using timer 4
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 */

#include "stm8_103.h"
#include "vectors.h"

#include "lib_bindec.h"
#include "lib_clock.h"

static char clock_ms;		/* milliseconds */
static char clock_tenths;	/* clock tenths: 0-9 */
static char clock_secs;		/* clock seconds: 0-59 */
static char clock_mins;		/* clock minutes: 0-59 */
static char clock_hours;	/* clock hours:   0-23 */
static char clock_days;		/* clock days:   0-255 */

static volatile char clock_lock;	/* lock to update clock */

static void (*timer_ms)(void);
static void (*timer_10)(void);

/******************************************************************************
 *
 * Initialize the clock (set up timer 4)
 * in: Millisecond callback, 1/10 second callback
 */

void clock_init(void (*call_ms)(void), void (*call_10)(void))
{
    timer_ms = call_ms;
    timer_10 = call_10;

    clock_ms = 0;
    clock_tenths = 0;
    clock_secs   = 0;
    clock_mins   = 0;
    clock_hours  = 0;
    clock_days   = 0;
    clock_lock   = 0;

    TIM4_PSCR = 6;	/* prescaler = 64 */
    TIM4_ARR  = 247;	/* reset and interrupt every 1.0 ms (TRIM: add 2) */
    TIM4_CR1  = 1;	/* enable timer4 */
    TIM4_IER  = 1;	/* enable timer4 interrupt */
}

/******************************************************************************
 *
 * Get string for current clock
 * Formats like "22:00:00" (8 chars + zero)
 */

void clock_string(char *buf)
{
    clock_lock = 1;
    bin8_dec2(clock_hours, buf);
    bin8_dec2(clock_mins,  buf + 3);
    bin8_dec2(clock_secs,  buf + 6);
    clock_lock = 0;
    buf[2] = ':';
    buf[5] = ':';
}

/******************************************************************************
 *
 *  Get binary values for day, hour, minute, second
 *  out: (4 bytes set)
 */

void clock_binary(char *vals)
{
    clock_lock = 1;
    vals[0] = clock_days;
    vals[1] = clock_hours;
    vals[2] = clock_mins;
    vals[3] = clock_secs;
    clock_lock = 0;
}

/******************************************************************************
 *
 *  Set clock from ASCII string (Format: "22:00:00")
 *  out: zero = success
 */

char clock_set(char *time)
{
    char	*t;
    char	ct, h, m, s;

    t = time;
    ct = 0;
    while (*t)
	if (*t++ == ':')
	    ct++;
    if (ct != 2)
	return 1;

    h = dec_bin16(time);
    while (*time++ != ':');
    m = dec_bin16(time);
    while (*time++ != ':');
    s = dec_bin16(time);

    if ((h > 23) ||
	(m > 59) ||
	(s > 59))
	return 1;
	    
    clock_lock = 1;
    clock_hours = h;
    clock_mins  = m;
    clock_secs  = s;
    clock_lock = 0;
    return 0;
}

/******************************************************************************
 *
 *  Timer 4 interrupt
 */

void timer4_isr(void) __interrupt (IRQ_TIM4)
{
    TIM4_SR = 0;		/* clear the interrupt */

    timer_ms();
    clock_ms++;
    if (clock_lock)
	return;
    if (clock_ms < 100)
	return;
    clock_ms -= 100;

    timer_10();
    clock_tenths++;
    if (clock_tenths < 10)
	return;
    clock_tenths = 0;

    clock_secs++;
//    clock_ms--;			/* TRIM: subtract 0.1% for this chip */
    if (clock_secs < 60)
	return;
    clock_secs = 0;
    clock_mins++;
    if (clock_mins < 60)
	return;
    clock_mins = 0;
    clock_hours++;
    if (clock_hours < 24)
	return;
    clock_hours = 0;
    clock_days++;
}
