/*
 *  File name:  lib_eeprom.c
 *  Date first: code clipped from lib_log
 *  Date last:  08/21/2018
 *
 *  Description: Library for EEPROM functions
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 */

#include "stm8s_header.h"

#include "lib_eeprom.h"

/******************************************************************************
 *
 * Unlock EEPROM for writing
 * out: zero = fail
 */
#pragma disable_warning 59

char eeprom_unlock(void)
{
__asm
    mov		_FLASH_DUKR, #0xae
    mov		_FLASH_DUKR, #0x56
    clr		a
00001$:
    dec		a
    jreq	00090$
    btjf	_FLASH_IAPSR, #3, 00001$
00090$:
__endasm;
}

/******************************************************************************
 *
 * Lock EEPROM from writing after write
 */

void eeprom_lock(void)
{
__asm
    bres	_FLASH_IAPSR, #3
__endasm;
}

