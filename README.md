# stm8_libs
Library collection for STM8, specifically stm8s103 and stm8s105.
My personal web pages are older and do not cover everything here,
but there are some nice pictures and links to buy cheap parts on eBay:
http://www.hodges.org/rh/stm8

My intent is to have a good working set of libraries to simplify the use of
internal peripherals (eg, UART1), give easier control (eg, PWM and servo
control), and support external devices more easily (eg, LCD & LED displays,
keypads, rotary encoders.)

The wiki pages document the functions in these libraries:
https://github.com/unfrozen/stm8_libs/wiki

I want each function to be as small as possible, so that even the stm8s103,
with only 8k of program memory, can do great things. Where it makes sense,
I use inline assembly to save bytes or when the C code is too ugly
(eg, bit operations). Once the code is debuggged, it can be used by anyone.
(I think using inline assembly in main code is a BAD IDEA.)

This code uses the SDCC compiler. Some of the libraries need to know which
CPU you are using, so please use -DSTM8103 or -DSTM8105 in the Makefile.

I am available for embedded projects of any size. You can contact me at:
richard@hodges.org

