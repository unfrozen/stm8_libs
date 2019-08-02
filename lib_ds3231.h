/*
 *  File name:  lib_ds3231.h
 *  Date first: 08/02/2019
 *  Date last:  08/02/2019
 *
 *  Description: Library for get/set datetime and temperature from DS3231
 *
 *  Author:     Jackkum
 *
 *  Copyright (C) 2018 Richard Hodges. All rights reserved.
 *  Permission is hereby granted for any use.
 *
 ******************************************************************************
 *
 *  RTC
 */

#include <stdint.h>

#define   TW_READ   1
#define   TW_WRITE   0

#define   TRUE   1
#define   FALSE   0

#define REG_SEC   0x00
#define REG_MIN   0x01
#define REG_HOUR  0x02
#define REG_DOW   0x03
#define REG_DATE  0x04
#define REG_MON   0x05
#define REG_YEAR  0x06
#define REG_CON   0x0e
#define REG_STATUS  0x0f
#define REG_AGING 0x10
#define REG_TEMPM 0x11
#define REG_TEMPL 0x12

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dayOfWeek;
  uint8_t dayOfMonth;
  uint8_t month;
  uint8_t year;
} DATETIME;

/*
 *  
 */
void ds3231_init(uint8_t address);

/*
 *  
 */
void ds3231_getTime(DATETIME *time);


/*
 *  
 */
void ds3231_setTime(DATETIME *time);

/*
 *  
 */
float ds3231_getTemp(void);