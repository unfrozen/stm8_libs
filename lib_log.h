/*
 *  File name:  lib_log.h
 *  Date first: 03/26/2018
 *  Date last:  04/16/2018
 *
 *  Description: Library for maintaining a many entry system log
 *
 *  Author:     Richard Hodges
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  Log entry
 */

typedef struct {
    unsigned long stamp;	/* non-zero, unique timestamp */
    char	type;		/* entry type */
    char	clock_h;	/* clock hour */
    char	clock_m;	/* clock minute */
    char	clock_s;	/* clock second */
} LOG_ENTRY;

#define LOG_STARTUP	1
#define LOG_DEVON	2
#define LOG_DEVOFF	3
#define LOG_TIMESET	4
#define LOG_NEWDAY	5

/*
 *  Initialize the log library
 *  in: base address, number of entries
 */
void log_init(char *, char);

/*
 *  Advance log timestamp
 *  (call every second)
 */
void log_second(void);

/*
 *  Erase all log entries
 */
void log_erase(void);

/*
 *  Write new log entry
 *  in: event type
 * out: zero = success
 */
char log_write(char);

/*
 *  Scan all log entries
 *  in: callback function for each entry
 */

void log_scan(void (*callback)(LOG_ENTRY *));

/*
 *  Unlock the EEPROM for writing
 * out: zero = fail
 */

char eeprom_unlock(void);

/*
 *  Lock EEPROM after writing
 */

void eeprom_lock(void);

/*
 *  Write word to EEPROM
 *  in: source, dest
 */

void eeprom_word(char *, char *);
