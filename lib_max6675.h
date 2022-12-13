/*
 *  File name:  lib_max6675.h
 *  Date first: 12/12/2022
 *  Date last:  12/12/2022
 *
 *  Description: STM8 Library for MAX6675 thermocouple converter.
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2022 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Types
 */
#include <stdint.h>

/*
 *  Initialize device
 */

void max6675_init(void);

/*
 *  Get current thermocouple temperature.
 *  out: temperature in 0.25 degrees C or  MAX6675_ERROR
 */

int16_t max6675_read(void);

#define MAX6675_ERROR (0x8000)
