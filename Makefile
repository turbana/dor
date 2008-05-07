CC    := gcc
LD    := ld
AS    := nasm
RM    := rm
DD    := dd
GRUB  := grub
CHKGRUB := scripts/chkgrub
MBCHK := mbchk
ECHO  := /bin/echo
CP    := cp
QEMU  := qemu
GDB   := gdb

SDIR := src
IDIR := include
BDIR := build

SRCS  := $(shell find $(SDIR) -mindepth 1 -name "*.c")
HDRS  := $(shell find $(IDIR) -mindepth 1 -name "*.h")
ASMS  := $(shell find $(SDIR) -mindepth 1 -name "*.asm")
OBJS  := $(notdir $(patsubst %.c,   %.o, $(SRCS)) \
				  $(patsubst %.asm, %.o, $(ASMS)))
BOBJS := $(addprefix $(BDIR)/, $(OBJS))
DEPS  := $(patsubst %.c, %.d, $(SRCS))
CLEAN := $(OBJS) $(DEPS) kernel.bin fdimage.img core* $(BDIR)

CFLAGS  := -Wall -Werror -O -g -fstrength-reduce -fomit-frame-pointer \
		   -finline-functions -nostdinc -fno-builtin -fno-stack-protector \
		   -pedantic -pedantic-errors -Wextra \
		   -Wno-variadic-macros \
		   -Wno-long-long \
		   -Wa,--fatal-warnings \
		   -I./$(IDIR)
LDFLAGS := --fatal-warnings
ASFLAGS := -f elf -g
QFLAGS  := -m 32 -no-kqemu

VPATH := $(SDIR)

# don't assume these targets are files
.PHONY: kernel all run debug version

# include dependencies created by gcc (*.d files)
-include $(DEPS)

all : run

kernel : kernel.bin

version :
	@echo "Build tools versions:"
	@echo "---------------------"
	@echo "KERNEL " `git tag`
	@echo "CC     " `$(CC) -v 2>&1 | tail -n1`
	@echo "LD     " `$(LD) -v`
	@echo "AS     " `$(AS) -v`
	@echo "QEMU   " `$(QEMU) | head -n1`
	@echo "GRUB   " `$(GRUB) --version`
	@echo "MAKE   " `$(MAKE) --version | head -n1`
	@echo "MTOOLS " `mtools --version | head -n1`

run : fdimage.img
	@echo "RUN  qemu"
	@$(QEMU) $(QFLAGS) -fda fdimage.img

debug : fdimage.img
	@echo "RUN  qemu debug"
	@$(QEMU) $(QFLAGS) -s -S -fda fdimage.img &
	@sleep 1
	@echo "GDB  kernel.bin"
	@$(GDB) kernel.bin

fdimage.img : $(BDIR) kernel.bin grub/stage1 grub/stage2 grub/menu.lst
	@echo "DD   $@"
	@$(DD) if=/dev/zero of=fdimage.img bs=512 count=2880 2> /dev/null
	@$(ECHO) 'drive a: file="fdimage.img"' > mtoolsrc
	@MTOOLSRC=mtoolsrc mformat -f 1440 a:
	@MTOOLSRC=mtoolsrc mmd   a:/boot
	@MTOOLSRC=mtoolsrc mmd   a:/boot/grub
	@MTOOLSRC=mtoolsrc mcopy grub/* a:/boot/grub/
	@MTOOLSRC=mtoolsrc mcopy kernel.bin a:/
	@$(ECHO) "(fd0) fdimage.img" > bmap
	@$(ECHO) -e "root (fd0)\nsetup (fd0)\nquit" | \
		$(GRUB) --batch --device-map=bmap > /dev/null
	@$(CHKGRUB) fdimage.img
	@$(RM) mtoolsrc bmap

grub/% :
	@echo "CP   $@"
	@$(CP) /boot/$@ $@

$(BDIR) :
	@echo "DIR  $@"
	@mkdir $@

kernel.bin : $(BOBJS) $(SDIR)/kernel.ld
	@echo "LINK $@"
	@$(LD) $(LDFLAGS) -T $(SDIR)/kernel.ld -o kernel.bin $(BOBJS)
	@$(MBCHK) kernel.bin > /dev/null

$(BDIR)/%.o : %.asm
	@echo "AS   $(<F)"
	@$(AS) $(ASFLAGS) $< -o $@

$(BDIR)/%.o : %.c $(IDIR)/%.h
	@echo "CC   $(<F)"
	@$(CC) $(CFLAGS) -MMD -MP -MT "build/$(@F)" -c $< -o $@

clean :
	-@for file in $(CLEAN); do \
		if [ -f $$file ]; then $(RM) $$file; fi; \
	done
	@$(RM) -rf $(BDIR)
