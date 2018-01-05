CC   = gcc -Wall -O2
SDCC = sdcc -mstm8 
SDAR = sdar
NAME = lib_stm8
LIB1 = lib_bindec
LIB2 = lib_rotary
LIB3 = lib_uart
LIB4 = lib_lcd
LIB5 = aes_stm8

all: $(NAME).lib

$(NAME).lib: $(LIB1).rel $(LIB2).rel $(LIB3).rel $(LIB4).rel $(LIB5).rel
	$(SDAR) rc $(NAME).lib $(LIB1).rel $(LIB2).rel $(LIB3).rel $(LIB4).rel $(LIB5).rel

$(LIB1).rel: $(LIB1).c
	$(SDCC) -c $(LIB1).c

$(LIB2).rel: $(LIB2).c
	$(SDCC) -c $(LIB2).c

$(LIB3).rel: $(LIB3).c
	$(SDCC) -c $(LIB3).c

$(LIB4).rel: $(LIB4).c
	$(SDCC) -c $(LIB4).c

$(LIB5).rel: $(LIB5).c
	$(SDCC) -c $(LIB5).c

clean:
	- rm -f *.adb *.asm *.cdb *.ihx *.lk *.lst *.map *.rel *.rst *.sym

