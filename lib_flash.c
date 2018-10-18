/*
 *  File name:  lib_flash.c
 *  Date first: code clipped from lib_log
 *  Date last:  10/17/2018
 *
 *  Description: Library for Flash memory functions
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 */

#include <string.h>

#include "stm8s_header.h"

#include "lib_flash.h"

static char p_flash_erase(char *);
static char r_flash_erase[30];

#pragma disable_warning 59

/******************************************************************************
 *
 *  Initialize Flash functions
 *  (Need to execute block erase from RAM)
 */

void flash_init(void)
{
    memcpy(r_flash_erase, p_flash_erase, 30);
}

/******************************************************************************
 *
 *  Erase FLASH block (64 or 128 bytes)
 *  in: block address
 * out: zero = success
 */

char flash_erase(char *ptr)
{
    ptr;
__asm
    ldw		x, (3, sp)
    clr		a
    push	cc
    sim
    call	_r_flash_erase
    pop		cc
__endasm;
}

static char p_flash_erase(char *ptr)
{
    ptr;
__asm
    bset	_FLASH_CR2, #5
    bres	_FLASH_NCR2, #5
    ld		(x), a
    ld		(1, x), a
    ld		(2, x), a
    ld		(3, x), a
    ldw		x, #4000*16/5
00001$:
    decw	x
    jreq	00090$
    btjt	_FLASH_CR2, #5, 00001$
    ret
00090$:
    inc		a
__endasm;
}

/******************************************************************************
 *
 * Unlock FLASH for writing
 * out: zero = fail
 */

char flash_unlock(void)
{
__asm
    mov		_FLASH_PUKR, #0x56
    mov		_FLASH_PUKR, #0xae
    clr		a
00001$:
    dec		a
    jreq	00090$
    btjf	_FLASH_IAPSR, #1, 00001$
00090$:
__endasm;
}

/******************************************************************************
 *
 * Lock FLASH from writing after write
 */

void flash_lock(void)
{
__asm
    bres	_FLASH_IAPSR, #1
__endasm;
}

/******************************************************************************
 *
 *  Clear range of Flash memory
 *  in: block base, size in bytes
 * out: zero = success
 */
char flash_clear(char *ptr, int size)
{
    char	retval;

    retval = flash_unlock();
    if (retval)
	return retval;
    while (((int)ptr & FLASH_BLOCK) & size) {
	*ptr = 0;
	ptr++;
	size--;
    }
    while (size >= FLASH_BLOCK) {
	retval = flash_erase(ptr);
	ptr  += FLASH_BLOCK;
	size -= FLASH_BLOCK;
    }
    while (size) {
	*ptr = 0;
	ptr++;
	size--;
    }
    flash_lock();
    return retval;
}
