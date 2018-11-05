/*
 *  File name:  lib_pins.h
 *  Date first: 11/04/2018
 *  Date last:  11/04/2018
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
    char	 reg_mask;	/* bit for row or column */
} IO_PIN;

