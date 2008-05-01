OUTPUT_FORMAT("elf32-i386")
ENTRY(entry)
virt = 0xC0000000;
phys = 0x00100000;
page = 0x00001000;

SECTIONS {
	.setup phys : AT(phys) {
		*(.setup)
	}
	.text virt + phys + page : AT(phys + page) {
		code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(page);
	}
	.data : AT(phys + page + (data - code)) {
		data = .;
		*(.data)
		. = ALIGN(page);
	}
	.bss : AT(phys + page + (bss - code)) {
		bss = .;
		*(.bss)
		. = ALIGN(page);
	}
	end = .;
}
