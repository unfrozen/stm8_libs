/*
 *  File name:  lib_rotary.h
 *  Date first: 12/29/2017
 *  Date last:  06/24/2018
 *
 *  Description: STM8 Library for ALPS rotary encoder
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Setup
 *  in: number of steps, LIMITS or ROLLOVER
 */
void alps_init(char steps, char option);

#define ALPS_LIMITS   1		/* value stops at zero, maximum steps */
#define ALPS_ROLLOVER 2		/* below zero and over maximum rolls over */

/*
 *  Get current position (relative from setup)
 */
char alps_value(void);

/*
 *  Poll switch (recommended: 1 to 4 milliseconds
 */
void alps_poll(void);
