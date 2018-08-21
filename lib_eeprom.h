/*
 *  File name:  lib_eeprom.h
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
 *
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
