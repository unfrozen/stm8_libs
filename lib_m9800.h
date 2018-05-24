/*
 *  File name:  lib_m9800.h
 *  Date first: 05/23/2018
 *  Date last:  05/23/2018
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
 *  Initialize
 *  out: zero = success
 */

char m9800_init(void);

/*
 *  Get current temperature
 *  out: 13-bit signed value, degrees C * 16
 */

short m9800_temp(void);
