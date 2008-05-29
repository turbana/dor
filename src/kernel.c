#include "kernel.h"
#include "sys.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isrs.h"
#include "timer.h"
#include "keyboard.h"
#include "kalloc.h"
#include "paging.h"
#include "asm.h"
#include "scheduler.h"
#include "mm.h"
#include "multiboot.h"
#include "assert.h"
#include "initrd.h"

#include "test_kalloc.h"
#include "test_tasks.h"

void
display_mboot(u32int mb_magic, struct multiboot_header *mboot) {
	struct multiboot_module *module;
	u32int i;
	u32int flags = mboot->flags;

	kprintf("mb_magic    = 0x%X\n", mb_magic);
	kprintf("flags       = 0x%X\n", mboot->flags);

	if(flags & MB_FLAG_MEM) {
		kprintf("mem_lower   = %dK\n", mboot->mem_lower);
		kprintf("mem_upper   = %dK\n", mboot->mem_upper);
	} else {
		kprintf("mem info not present\n");
	}

	if(flags & MB_FLAG_DEVICE) {
		kprintf("boot_device = 0x%X\n", mboot->boot_device);
	} else {
		kprintf("boot device info not present\n");
	}

	if(flags & MB_FLAG_CMDLINE) {
		kprintf("cmdline     = %s\n", (char *)mboot->cmdline);
	} else {
		kprintf("command line not present\n");
	}

	if(flags & MB_FLAG_MODS) {
		kprintf("mods_count  = %d\n", mboot->mods_count);
		module = (struct multiboot_module *)mboot->mods_addr;
		for(i = 0; i < mboot->mods_count; i++, module++) {
			kprintf(" module:    %s\n", module->string);
			kprintf(" mod_start: 0x%X\n", module->mod_start);
			kprintf(" mod_end:   0x%X\n", module->mod_end);
		}
	} else {
		kprintf("modules not present\n");
	}

	if(flags & MB_FLAG_AOUT) {
		kprintf("kernel is aout binary\n");
	} else if(flags & MB_FLAG_ELF) {
		kprintf("kernel is elf binary\n");
	} else {
		kprintf("kernel is unknown binary\n");
	}
}


void
load_modules(struct multiboot_module *module, u32int count) {
	u32int i;

	for(i = 0; i < count; i++, module++) {
		if(!strcmp((char *)module->string, "/initrd.bin")) {
			initrd_init(module->mod_start);
		}
	}
}


void
k_entry(u32int mb_magic, struct multiboot_header *mboot) {
	paging_init();
	gdt_init();
	kalloc_init();
	screen_init();

	if(!test_kalloc_suite()) {
		ASM("cli\n\t"
			"hlt");
	}

	idt_init();
	isrs_init();
	irq_init();
	timer_init();
	mm_init(32 * 1024 * 1024);
	scheduler_init();
	keyboard_init();

	/* ensure we have a correct multiboot header */
	ASSERT(mb_magic == MB_HEADER_MAGIC);
	display_mboot(mb_magic, mboot);

	load_modules((struct multiboot_module *)mboot->mods_addr,
				 mboot->mods_count);

	kprintf("Hello Ian.\n");

	/* setup some test tasks */
/*	task_create(task1);
	task_create(task2);
	task_create(task3);
	task_create(task4);
	task_create(task5);*/

	initrd_list();

	/* enable interrupts and start scheduling tasks */
	scheduler_start();	/* never exits */
}
