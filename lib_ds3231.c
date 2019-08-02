#include "stm8s_header.h"
#include "lib_i2c.h"
#include "lib_ds3231.h"

// Adress of current device
uint8_t deviceAddress = 0;

/*
 * Initialize the device
 */
void ds3231_init(uint8_t address) {
	if( ! deviceAddress){
		i2c_init();
	}

	deviceAddress = address;
}

uint8_t decToBcd(uint8_t val) {
  return ( (val/10*16) + (val%10) );
}

uint8_t bcdToDec(uint8_t val) {
  return ( (val/16*10) + (val%16) );
}

uint8_t startWrite(uint8_t address) {
	uint8_t wa = TW_WRITE | (deviceAddress << 1);
	uint8_t ra = TW_READ | wa;

	i2c_start();

  i2c_txbit8(wa);
  if ( ! i2c_getack()) {
    return FALSE;
  }
  
  i2c_txbit8(address);
  if ( ! i2c_getack()) {
    return FALSE;
  }

  return TRUE;
}

uint8_t startRead(uint8_t address) {
	uint8_t wa = TW_WRITE | (deviceAddress << 1);
	uint8_t ra = TW_READ | wa;

	i2c_start();

  i2c_txbit8(wa);
  if ( ! i2c_getack()) {
    return FALSE;
  }
  
  i2c_txbit8(address);
  if ( ! i2c_getack()) {
    return FALSE;
  }

  i2c_stop();

  i2c_start();
  i2c_txbit8(ra);
  if ( ! i2c_getack()) {
    return FALSE;
  }
  
  i2c_data1();

  return TRUE;
}

void stop(void) {
	i2c_stop();
}

/*
 *  Get current datetime value
 */
void ds3231_getTime(DATETIME *time) {
	
	if(startRead(REG_SEC)) {
		time->second = bcdToDec(i2c_rxbit8() & 0x7f);
	  i2c_sendack();

	  time->minute     = bcdToDec(i2c_rxbit8());
	  i2c_sendack();

	  time->hour       = bcdToDec(i2c_rxbit8() & 0x3f); 
	  i2c_sendack();

	  time->dayOfWeek  = bcdToDec(i2c_rxbit8());
	  i2c_sendack();

	  time->dayOfMonth = bcdToDec(i2c_rxbit8());
	  i2c_sendack();

	  time->month      = bcdToDec(i2c_rxbit8());
	  i2c_sendack();

	  time->year       = bcdToDec(i2c_rxbit8());
	  i2c_sendnak();
	}

  stop();
}


/*
 *  Set new datetime value
 */
void ds3231_setTime(DATETIME *time) {
	if(startWrite(REG_SEC)) {
		i2c_txbit8(decToBcd(time->second));
		i2c_getack();

		i2c_txbit8(decToBcd(time->minute));
		i2c_getack();

		i2c_txbit8(decToBcd(time->hour));
		i2c_getack();

		i2c_txbit8(decToBcd(time->dayOfWeek));
		i2c_getack();

		i2c_txbit8(decToBcd(time->dayOfMonth));
		i2c_getack();

		i2c_txbit8(decToBcd(time->month));
		i2c_getack();

		i2c_txbit8(decToBcd(time->year));
		i2c_getack();
	}

  stop();
}

/*
 *  Read temperature from module
 */
float ds3231_getTemp(void) {
	uint8_t _msb = 0, _lsb = 0;

	if(startRead(REG_TEMPM)) {
		_msb = i2c_rxbit8();
		i2c_sendack();

		_lsb = i2c_rxbit8();
		i2c_sendnak();
	}

	stop();
	return (float)_msb + ((_lsb >> 6) * 0.25f);
}