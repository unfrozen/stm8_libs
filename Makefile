CC   = cc -Wall -O2
SDCC = sdcc -mstm8 -DSTM8103
SDAR = sdar
NAME = lib_stm8
OBJS = lib_bindec.rel lib_rotary.rel lib_uart.rel lib_lcd.rel\
	aes_stm8.rel lib_cap2.rel lib_max7219.rel lib_cli.rel\
	lib_clock.rel lib_log.rel lib_i2c.rel lib_m9800.rel lib_tm1638.rel \
	lib_pwm.rel lib_eeprom.rel lib_adc.rel lib_keypad.rel lib_flash.rel \
	lib_delay.rel lib_ping.rel lib_tm1637.rel lib_w1209.rel \
	lib_board.rel lib_spi.rel lib_tim4.rel lib_max6675.rel

.SUFFIXES : .rel .c

all: $(NAME).lib

$(NAME).lib: $(OBJS)
	$(SDAR) rc $(NAME).lib $?

.c.rel :
	$(SDCC) -c $<

clean:
	- rm -f *.adb *.asm *.cdb *.ihx *.lk *.lst *.map *.rel *.rst *.sym \
	aes_tables aes_tables.h

aes_stm8.rel: aes_stm8.c aes_tables.h
	$(SDCC) -c aes_stm8.c

aes_tables: aes_tables.c
	$(CC) -o aes_tables aes_tables.c

aes_tables.h: aes_tables
	./aes_tables > aes_tables.h

