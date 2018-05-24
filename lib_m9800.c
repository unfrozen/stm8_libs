/*
 *  File name:  lib_m9800.c
 *  Date first: 05/23/2018
 *  Date last:  05/24/2018
 *
 *  Description: Library for reading temperature from Microchip MCP9800 device.
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

#include "lib_i2c.h"
#include "lib_m9800.h"

#define I2C_ADDR	7	/* from address pins a0, a1, a2 */
#define I2C_DEV_ADDR	(0x90 | (I2C_ADDR << 1))

#define CONFIG_VAL	0x60	/* continuous conversion, max precision */

#define REG_TEMP	0
#define REG_CONFIG	1
#define REG_HYSTERESIS	2
#define REG_TEMP_LIMIT	3

/******************************************************************************
 *
 *  Initialize
 *  out: zero = success
 */

char m9800_init(void)
{
    i2c_start();
    i2c_txbit8(I2C_DEV_ADDR);
    if (!i2c_getack()) {
	i2c_stop();
	return 1;
    }
    i2c_txbit8(REG_CONFIG);
    i2c_getack();
    i2c_txbit8(CONFIG_VAL);
    i2c_stop();
    return 0;
}

/******************************************************************************
 *
 *  Get current temperature
 *  out: 13-bit signed value, degrees C * 16
 */

short m9800_temp(void)
{
    short	temp;
    char	val;

    temp = 0;

    i2c_start();
    i2c_txbit8(I2C_DEV_ADDR);
    if (!i2c_getack())
	goto quit;
    i2c_txbit8(REG_TEMP);
    if (!i2c_getack())
	goto quit;
    i2c_stop();

    i2c_start();
    i2c_txbit8(I2C_DEV_ADDR | 1);
    if (!i2c_getack())
	goto quit;
    i2c_data1();		/* release control of data pin */

    val = i2c_rxbit8();
    temp = val << 8;
    i2c_sendack();

    val = i2c_rxbit8();
    temp |= val;
    i2c_sendnak();
quit:
    i2c_stop();

    return temp >> 4;		/* note: signed shift */
}

