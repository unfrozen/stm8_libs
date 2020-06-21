/*
 *  File name:  lib_clock.h
 *  Date first: 03/23/2018
 *  Date last:  06/22/2020
 *
 *  Description: Library for maintaining a wall clock using timer 4
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018-2020 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Option to extend clock to handle calendar date
 *  Comment out to save code and memory.
 */

#define CLOCK_CALENDAR

/*
 * Initialize the clock (set up timer 4)
 * in: Millisecond callback
 */

void clock_init(void (*)(void), void (*)(void));

/*
 * Get string for current clock
 * Formats like "22:00:00" (8 chars + zero)
 */

void clock_string(char *);

/*
 *  Set clock from ASCII string
 *  Format: "22:00:00"
 */

char clock_set(char *);

/*
 *  Get/set binary values for day, hour, minute, second
 *  out: (4 bytes set)
 */

void clock_bin_get(char *);
void clock_bin_set(char *);

/*
 *  Set large and fine clock trim
 *  Positive values speed up, negative values slow down
 *  in: large +/- (0.4%), fine +/- (0.1%)
 */

void clock_trim(signed char, signed char);

/*
 *  Interrupt service routine declaration
 */
#include "vectors.h"

void timer4_isr(void) __interrupt (IRQ_TIM4);

/*
 *  OPTIONAL CALENDAR FUNCTIONS
 */
#ifdef CLOCK_CALENDAR
typedef struct {
    int		year;		/* 2000-2199 */
    char	month;		/* 1-12 */
    char	date;		/* 1-31 */
    char	day;		/* 1-7 */
} CLOCK_CAL;

/*
 *  Get or set current year, month, date, day of week
 */
void clock_cal_get(CLOCK_CAL *);
void clock_cal_set(CLOCK_CAL *);

/*
 *  Advance the calendar date (for testing calendar function)
 */
void clock_inc_calendar(void);

#endif	/* CLOCK_CALENDAR */
