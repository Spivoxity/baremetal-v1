# x04-numbers/Makefile
# Copyright (c) 2020-21 J. M. Spivey

# genmake x04-numbers max.hex part1 {fmain.o lib.o}

all: max.hex

CC = arm-none-eabi-gcc
CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall -ffreestanding
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.elf: %.o fmain.o lib.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T nRF51822.ld \
		$^ -nostdlib -lgcc -o $@ -Wl,-Map,$*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile '$@'"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

clean:
	rm -f *.hex *.elf *.bin *.map *.o

# Don't delete intermediate files
.SECONDARY:

###

fmain.o: lib.h hardware.h
lib.o: lib.h
startup.o: hardware.h
