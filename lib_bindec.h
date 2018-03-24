/*
 *  File name:  lib_bindec.h
 *  Date first: 12/24/2017
 *  Date last:  03/23/2018
 *
 *  Description: Library of binary/decimal functions for STM8
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2017, 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Convert 16 bit unsigned binary to 5 digit decimal, terminate with zero
 */

void bin16_dec(short, char *);

/*
 *  Convert 32 bit unsigned binary to 10 digit decimal, terminate with zero
 */

void bin32_dec(long, char *);

/*
 *  Trim leading zeroes from decimal buffer
 *  in:  buffer, maximum zeroes to remove
 *  out: first non-zero digit
 */

char *decimal_rlz(char *, char);

/*
 *  Convert binary byte to hex, terminate with zero
 */

void bin8_hex(char, char *);

/*
 *  Convert ASCII decimal digits to 16-bit binary
 *  Stops on first non-decimal character
 */

int dec_bin16(char *);
