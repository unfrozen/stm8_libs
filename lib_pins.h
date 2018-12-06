/*
 *  File name:  lib_pins.h
 *  Date first: 11/04/2018
 *  Date last:  11/14/2018
 *
 *  Description: STM8 Library to simplify pin usage.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 * Structured pin definition
 */
typedef struct {
    volatile char *reg_base;	/* port ODR register */
    char	 reg_mask;	/* bit to use */
} IO_PIN;

/* Define one pin and a callback function */

typedef struct {
    IO_PIN	*pin;
    void	(*callback)(void);
} IO_CALL;

/* Define one pin and a callback function that gets int */

typedef struct {
    IO_PIN	*pin;
    void	(*callback)(int);
} IO_CALL_INT;
