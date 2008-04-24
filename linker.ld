OUTPUT_FORMAT("binary")		/* we want a flat file */
ENTRY(entry)			/* where do we first jump to? */
phys = 0x00100000;		/* load us at 1mb */
SECTIONS {
	.text phys : AT(phys) {
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN 4096;
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
