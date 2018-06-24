CC   = gcc -Wall -O2
SDCC = sdcc -mstm8 -DSTM8103
SDAR = sdar
NAME = lib_stm8
OBJS = lib_bindec.rel lib_rotary.rel lib_uart.rel lib_lcd.rel\
	aes_stm8.rel lib_cap2.rel lib_max7219.rel lib_cli.rel\
	lib_clock.rel lib_log.rel lib_i2c.rel lib_m9800.rel lib_tm1638.rel

.SUFFIXES : .rel .c

all: $(NAME).lib

$(NAME).lib: $(OBJS)
	$(SDAR) rc $(NAME).lib $?

.c.rel :
	$(SDCC) -c $<

clean:
	- rm -f *.adb *.asm *.cdb *.ihx *.lk *.lst *.map *.rel *.rst *.sym

