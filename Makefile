CC = gcc-3.3
LD = ld
AS = nasm
RM = rm

CFLAGS  = -Wall -Werror -O -g -fstrength-reduce -fomit-frame-pointer \
	-finline-functions -nostdinc -fno-builtin -I./include
LDFLAGS =
ASFLAGS = -f aout

OBJS = entry.o main.o screen.o system.o

all : kernel.bin

run : kernel.bin
	qemu -fda kernel.bin -m 32

kernel.bin : $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin $(OBJS)

entry.o : entry.asm
	$(AS) $(ASFLAGS) -o entry.o entry.asm

%.o : %.c include/%.h
	$(CC) $(CFLAGS) -c $<

clean :
	-$(RM) kernel.bin *.o
