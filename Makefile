CC=gcc-3.3
LD=ld
AS=nasm
RM=rm

CFLAGS=-Wall -Werror -g -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include
LDFLAGS=

all : kernel

kernel : entry.o main.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin entry.o main.o

entry.o : entry.asm
	$(AS) -f aout -o entry.o entry.asm

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	-$(RM) kernel.bin *.o
