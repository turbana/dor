CC=gcc-3.3
LD=ld
AS=nasm
RM=rm

CFLAGS=-Wall -Werror -g -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include
LDFLAGS=

all : kernel system.o

kernel : entry.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin entry.o

entry.o : entry.asm
	$(AS) -f aout -o entry.o entry.asm

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-$(RM) kernel.bin *.o
