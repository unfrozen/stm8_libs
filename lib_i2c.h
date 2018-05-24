/*
 *  File name:  lib_i2c.h
 *  Date first: 05/17/2018
 *  Date last:  05/23/2018
 *
 *  Description: Library for communicating with I2C devices.
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

void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);

char i2c_rxbit8(void);
void i2c_txbit8(char);
char i2c_getack(void);
void i2c_sendack(void);
void i2c_sendnak(void);

void i2c_clock0(void);
void i2c_clock1(void);
void i2c_data0(void);
void i2c_data1(void);
