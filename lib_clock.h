/*
 *  File name:  lib_clock.h
 *  Date first: 03/23/2018
 *  Date last:  04/14/2018
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
 * Initialize the clock (set up timer 4)
 * in: Millisecond callback
 */

void clock_init(void (*void )(void), void (*void)(void));

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
 *  Get binary values for day, hour, minute, second
 *  out: (4 bytes set)
 */

void clock_binary(char *);

/*
 *  Set large and fine clock trim
 *  Positive values speed up, negative values slow down
 *  in: large +/- (0.4%), fine +/- (0.1%)
 */

void clock_trim(signed char, signed char);

/*  DEBUGGING: Expose the clock trim values  */

static signed char	trim_large;
static signed char	trim_fine;

/*
 *  Interrupt service routine declaration
 */
#include "vectors.h"

void timer4_isr(void) __interrupt (IRQ_TIM4);

