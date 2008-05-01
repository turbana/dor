OUTPUT_FORMAT("elf32-i386")
ENTRY(entry)
virt = 0xC0000000;
phys = 0x00101000;

SECTIONS {
	.setup ALIGN(4) : AT(phys - 4096) {
		*(.setup)
	}
	.text virt : AT(phys) {
		code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}
	.data : AT(phys + (data - code)) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(phys + (bss - code)) {
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	end = .;
}
