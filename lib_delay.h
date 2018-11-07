/*
 *  File name:  lib_delay.h
 *  Date first: 11/05/2018
 *  Date last:  11/05/2018
 *
 *  Description: STM8 Library for short delays.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 * Delays
 */
void delay_500ns(void);		/* 500 microseconds */
void delay_usecs(char);		/* 1 to 255 microseconds */
void delay_50us(void);		/* 50 microseconds */
void delay_ms(unsigned char);	/* 1 to 255 milliseconds (setup code) */
