/*OUTPUT_FORMAT("elf32-i386")*/		/* we need to be a flat binary */
OUTPUT_FORMAT("binary")		/* we need to be a flat binary */
ENTRY(entry)			/* where do we first jump to? */
phys = 0xC0000000;		/* load us at RAM - 256mb */
low  = 0x00100000;		/* load the entry point at 1mb */
SECTIONS {
	.setup : AT(low) {
		*(.setup)
	}
	.text phys : AT(ADDR(.text) - phys) {
		code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - phys) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss) - phys) {
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	end = .;
}

/*SECTIONS {
	. = low;

	.setup : {
		*(.setup)
	}

	. += phys;

	.text ALIGN(4096) : AT(ADDR(.text) - phys) {
		code = .;
		*(.text*)
		. = ALIGN(4096);
	}

	.data ALIGN(4096) : AT(ADDR(.data) - phys) {
		data = .;
		*(.data*)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.bss ALIGN(4096) : AT(ADDR(.bss) - phys) {
		bss = .;
		*(.bss*)
		*(COMMON*)
		. = ALIGN(4096);
	}
	end = .;
}*/
