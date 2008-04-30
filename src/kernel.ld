OUTPUT_FORMAT("binary")		/* we need to be a flat binary */
ENTRY(entry16)			/* where do we first jump to? */
phys = 0xF0000000;		/* load us at RAM - 256mb */
SECTIONS {
	.text phys : AT(phys) {
		code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}
	.data : AT(phys + (data - code) + 0x6000) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(phys + (bss - code) + 0x6000) {
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	end = .;
}
