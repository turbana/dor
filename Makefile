CC = gcc
LD = ld
AS = nasm
RM = rm
DD = dd
GRUB = grub
MBCHK = mbchk

CFLAGS  = -Wall -Werror -O -g -fstrength-reduce -fomit-frame-pointer \
		  -finline-functions -nostdinc -fno-builtin -fno-stack-protector \
		  -I./include
LDFLAGS =
ASFLAGS = -f aout

OBJS = boot.o kernel.o screen.o sys.o gdt.o idt.o isrs.o irq.o timer.o \
	   keyboard.o kalloc.o

all : run

kernel : kernel.bin

run : fdimage.img
	qemu -fda fdimage.img

fdimage.img : kernel.bin grub/stage1 grub/stage2 grub/menu.lst
	$(DD) if=/dev/zero of=fdimage.img bs=512 count=2880
	echo 'drive a: file="fdimage.img"' > mtoolsrc
	MTOOLSRC=mtoolsrc mformat -f 1440 a:
	MTOOLSRC=mtoolsrc mmd   a:/boot
	MTOOLSRC=mtoolsrc mmd   a:/boot/grub
	MTOOLSRC=mtoolsrc mcopy grub/* a:/boot/grub/
	MTOOLSRC=mtoolsrc mcopy kernel.bin a:/
	echo "(fd0) fdimage.img" > bmap
	echo -e "root (fd0)\nsetup (fd0)\nquit" | $(GRUB) --batch --device-map=bmap
	$(RM) mtoolsrc bmap

grub/% :
	cp /boot/$@ ./$@

kernel.bin : $(OBJS)
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel.bin $(OBJS)
	$(MBCHK) kernel.bin > /dev/null

boot.o : boot.asm
	$(AS) $(ASFLAGS) -o boot.o boot.asm

%.o : %.c include/%.h
	$(CC) $(CFLAGS) -c $<

clean :
	-$(RM) kernel.bin *.o fdimage.img grub/stage[12]
