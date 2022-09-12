/*
 *  File name:  lib_log.c
 *  Date first: 03/26/2018
 *  Date last:  10/16/2018
 *
 *  Description: Library for using a many entry system log
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 * Note: This system log library uses the Timer4 clock library to
 * save the current clock as timestamps.
 */

#include "stm8s_header.h"

#include "lib_log.h"
#include "lib_clock.h"
#include "lib_eeprom.h"
#include "lib_flash.h"

static LOG_ENTRY *log_base;
static LOG_ENTRY *log_end;
static LOG_ENTRY *log_old;	/* oldest log entry */
static LOG_ENTRY *log_new;	/* newest log entry */

static int  log_size;		/* number bytes in each entry */
static short log_count;		/* number of log entries */

static long log_stamp;

static void (*mem_lock)(void);
static char (*mem_unlock)(void);

/******************************************************************************
 *
 * Initialize the system log
 * in: base address, number of enties
 */

void log_init(char *base, short count)
{
    LOG_ENTRY	*old, *new, *ptr;
    short	i;

    log_base  = (LOG_ENTRY *)base;
    log_end   = log_base + count;
    log_count = count;
    log_size  = sizeof(LOG_ENTRY);
    log_stamp = 0;

    old = log_base;
    new = log_base;
    ptr = log_base;

    for (i = 0; i < count; i++) {
	ptr++;
	if (!ptr->stamp)
	    continue;
	if (old->stamp > ptr->stamp)
	    old = ptr;
	if (log_stamp < ptr->stamp) {
	    log_stamp = ptr->stamp;
	    new = ptr;
	}
    }
    log_stamp++;
    log_old = old;
    log_new = new;

    if (((short)base > 0x8000)) {
	mem_lock   = flash_lock;
	mem_unlock = flash_unlock;
    }
    else {
	mem_lock   = eeprom_lock;
	mem_unlock = eeprom_unlock;
    }
}

/******************************************************************************
 *
 *  Erase all log entries
 */
void log_erase(void)
{
    char	*ptr, zero[4];
    short	i;

    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    zero[3] = 0;

    ptr = (char *)log_base;
    i = log_count;

    if (!mem_unlock())
	return;
    while (i) {
	eeprom_word(zero, ptr);
	ptr += 4;
	eeprom_word(zero, ptr);
	ptr += 4;
	i--;
    }
    mem_lock();
    log_stamp = 0;
    log_old = log_base;
    log_new = log_base;
}

/******************************************************************************
 *
 *  Write new log entry
 *  in: event type
 * out: zero = success
 */
char log_write(char type)
{
    LOG_ENTRY	entry;
    char	binary[4], *src, *dst;

    clock_bin_get(binary);
    entry.stamp = log_stamp;
    entry.type  = type;
    entry.clock_h = binary[1];
    entry.clock_m = binary[2];
    entry.clock_s = binary[3];

    if (!mem_unlock())
	return 1;

    log_new++;
    if (log_new == log_end)
	log_new =  log_base;
    if (log_new == log_old) {
	log_old++;
	if (log_old == log_end)
	    log_old =  log_base;
    }
    dst = (char *)log_new;
    src = (char *)&entry;

    eeprom_word(src, dst);
    eeprom_word(src + 4, dst + 4);
    mem_lock();

    return 0;
}

/******************************************************************************
 *
 *  Advance log timestamp
 */

void log_second(void)
{
__asm
    ldw		x, _log_stamp+2
    incw	x
    ldw		_log_stamp+2, x
    jreq	00001$
    ret
00001$:
    ldw		x, _log_stamp+0
    incw	x
    ldw		_log_stamp+0, x
    ret
__endasm;
}

/******************************************************************************
 *
 *  Scan all log entries
 *  in: callback function for each entry
 */

void log_scan(void (*callback)(LOG_ENTRY *))
{
    LOG_ENTRY *entry;

    entry = log_old;
    while (1) {
	if (entry->stamp)
	    callback(entry);
	entry++;
	if (entry == log_end)
	    entry =  log_base;
	if (entry == log_old)
	    break;
    }
}

/******************************************************************************
 *
 *  Get count of valid log entries
 */

short log_valid(void)
{
    LOG_ENTRY	*entry;
    short	count;

    entry = log_base;
    count = 0;

    while (entry != log_end) {
	if (entry->stamp)
	    count++;
	entry++;
    }
    return count;
}

/******************************************************************************
 *
 *  Do EEPROM word write
 */

void eeprom_word(char *src, char *dst)
{
    src, dst;
__asm
    bset	_FLASH_CR2, #6
    bres	_FLASH_NCR2, #6
    ldw		x, (3, sp)
    ldw		y, (5, sp)
    push	#4
00001$:
    ld		a, (x)
    ld		(y), a
    incw	x
    incw	y
    dec		(1, sp)
    jrne	00001$
    pop		a
__endasm;
}
