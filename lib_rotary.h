/*
 *  File name:  lib_rotary.h
 *  Date first: 12/29/2017
 *  Date last:  12/29/2017
 *
 *  Description: STM8 Library for ALPS rotary encoder
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Setup
 *  in: number of steps
 */
void alps_init(char steps);

/*
 *  Get current position (relative from setup)
 */
char alps_value(void);

/*
 *  Poll switch (recommended: 1 to 4 milliseconds
 */
void alps_poll(void);
