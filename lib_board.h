/*
 *  File name:  lib_board.h
 *  Date first: 07/01/2019
 *  Date last:  07/01/2019
 *
 *  Description: STM8 Library for popular board support (stm8s103, stm8s105).
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2019 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize board
 *
 *  Board frequency override may be added later.
 *  Default is 16mhz internal for stm8s103 and 8mhz external xtal for stm8s105.
 */

void board_init(char freq);

/*
 *  Turn LED on or off.
 *  in: zero = off, non-zero = on
 */

void board_led(char on);
