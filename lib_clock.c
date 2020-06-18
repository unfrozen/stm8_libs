/*
 *  File name:  lib_clock.c
 *  Date first: 03/23/2018
 *  Date last:  04/18/2018
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

#include "stm8s_header.h"

#include "lib_bindec.h"
#include "lib_clock.h"

static signed char clock_ms;	/* milliseconds, may be negative with trim */
static char clock_tenths;	/* clock tenths: 0-9 */
static char clock_secs;		/* clock seconds: 0-59 */
static char clock_mins;		/* clock minutes: 0-59 */
static char clock_hours;	/* clock hours:   0-23 */
static char clock_days;		/* clock days:   0-255 */

static signed char trim_second;	/* small trim every second */

static volatile char clock_lock;	/* lock to update clock */

#ifdef CLOCK_CALENDAR
static CLOCK_CAL calendar;
#endif

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

    trim_second = 0;		/* millisecond trim every second */

#ifdef STM8103
    TIM4_PSCR = 6;	/* prescaler = 64 for 16mhz */
#endif
#ifdef STM8105
    TIM4_PSCR = 5;	/* prescaler = 32 for 8mhz */
#endif
    TIM4_ARR  = 249;	/* reset and interrupt every 1.0 ms */
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

void clock_bin_get(char *vals)
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
 *  Set clock from binary values
 *  out: (4 bytes set)
 */

void clock_bin_set(char *vals)
{
    clock_lock = 1;
    clock_days  = vals[0];
    clock_hours = vals[1];
    clock_mins  = vals[2];
    clock_secs  = vals[3];
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
 *  Set large and fine clock trim
 *  in: large +/- (0.4%), fine +/- (0.1%)
 */

void clock_trim(signed char large, signed char fine)
{
    TIM4_ARR = 249 - large;
    trim_second = fine;
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

    clock_ms += trim_second;
    clock_secs++;
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
    
#ifdef CLOCK_CALENDAR
    clock_inc_calendar();
#endif
}

#ifdef CLOCK_CALENDAR

static const char days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/******************************************************************************
 *
 *  Advance calendar one day
 */

void clock_inc_calendar(void)
{
    char	mdays;
    
    calendar.day++;
    if (calendar.day > 7)
	calendar.day = 1;

    mdays = days[calendar.month - 1];
    if (calendar.month == 2 &&
	(calendar.year & 3) == 0)
	mdays++;
    calendar.date++;
    if (calendar.date <= mdays)
	return;
    calendar.date = 1;

    calendar.month++;
    if (calendar.month < 13)
	return;
    calendar.month = 1;

    calendar.year++;
}

/* NOTE: This function will normally be called from the timer interrupt, and
 * will be protected by "clock_lock". During testing, this may also be called
 * from user space and that code will need to avoid possible race condition.
 *
 ******************************************************************************
 *
 *  Get calendar data
 *  in: calendar structure
 */

void clock_cal_get(CLOCK_CAL *cal)
{
    clock_lock = 1;
    cal->year  = calendar.year;
    cal->month = calendar.month;
    cal->date  = calendar.date;
    cal->day   = calendar.day;
    clock_lock = 0;
}

/******************************************************************************
 *
 *  Set calendar data
 *  in: calendar structure
 */

void clock_cal_set(CLOCK_CAL *cal)
{
    clock_lock = 1;
    calendar.year  = cal->year;
    calendar.month = cal->month;
    calendar.date  = cal->date;
    calendar.day   = cal->day;
    clock_lock = 0;
}

#endif	/* CLOCK_CALENDAR */
