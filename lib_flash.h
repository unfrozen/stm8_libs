/*
 *  File name:  lib_flash.h
 *  Date first: code clipped from lib_log
 *  Date last:  10/18/2018
 *
 *  Description: Library for Flash memory functions
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Initialize Flash functions
 */
void flash_init(void);

/*
 *  Unlock the Flash for writing
 * out: zero = fail
 */

char flash_unlock(void);

/*
 *  Lock Flash after writing
 */

void flash_lock(void);

#define FLASH_BLOCK_ERASE /* comment out to save setup and 30 bytes of RAM */

/*
 *  Erase FLASH block (64 or 128 bytes)
 *  in: block address
 * out: zero = success
 */

char flash_erase(char *);

/*
 *  Clear range of Flash memory
 *  in: block base, size in bytes
 * out: zero = success
 */
char flash_clear(char *, int);
