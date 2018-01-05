/*
 *  File name:  lib_bindec.h
 *  Date first: 12/24/2017
 *  Date last:  12/24/2017
 *
 *  Description: Library of binary/decimal functions for STM8
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Convert 16 bit unsigned binary to 5 digit decimal
 */

void bin16_dec(short, char *);

/*
 *  Convert 32 bit unsigned binary to 10 digit decimal
 */

void bin32_dec(long, char *);

/*
 *  Trim leading zeroes from decimal buffer
 *  in:  buffer, maximum zeroes to remove
 *  out: first non-zero digit
 */

char *decimal_rlz(char *, char);
