[BITS 32]
global start
start:
	extern k_entry
	mov esp, _sys_stack	; set up stack pointer
	call k_entry		; jump to our entry point
	hlt

ALIGN 4				; make sure we are 4 byte alligned
mboot:
	extern code, bss, end
	MB_PAGE_ALIGN	equ 1<<0
	MB_MEM_INFO	equ 1<<1
	MB_AOUT_KLUDGE	equ 1<<16
	MB_HEAD_MAGIC	equ 0x1BADB002
	MB_HEAD_FLAGS	equ MB_PAGE_ALIGN | MB_MEM_INFO | MB_AOUT_KLUDGE
	MB_CHECKSUM	equ -(MB_HEAD_MAGIC + MB_HEAD_FLAGS)

	dd MB_HEAD_MAGIC	; GRUB multiboot header
	dd MB_HEAD_FLAGS
	dd MB_CHECKSUM

	dd mboot		; AOUT kludge - must be physical addresses
	dd code			; the linker will fill these in
	dd bss
	dd end
	dd start

SECTION .bss
	resb 8192		; reserve 4kb for our stack
_sys_stack:			; and set our stack pointer to the end