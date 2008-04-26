[BITS 32]


;;; ENTRY POINT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global start
start:
	extern k_entry
	mov esp, _sys_stack	; set up stack pointer
	call k_entry		; jump to our entry point
	hlt


;;; MUTLTI BOOT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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


;;; GDT SETUP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global gdt_flush		; setup the GDT
extern gp
gdt_flush:
	lgdt [gp]		; load the GDT with our GDT pointer struct
	mov ax, 0x10		; 0x10 is our data segment in the GDT
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush		; far jump into our code segment
flush:	ret


;;; IDT SETUP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global idt_load			; setup the IDT
extern idtp
idt_load:
	lidt [idtp]
	ret


;;; ISR ROUTINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common

isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common

isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common

isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common

isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common

isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common

isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common

isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common

isr8:
	cli
	push byte 8
	jmp isr_common

isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common

isr10:
	cli
	push byte 10
	jmp isr_common

isr11:
	cli
	push byte 11
	jmp isr_common

isr12:
	cli
	push byte 12
	jmp isr_common

isr13:
	cli
	push byte 13
	jmp isr_common

isr14:
	cli
	push byte 14
	jmp isr_common

isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common

isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common

isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common

isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common

isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common

isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common

isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common

isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common

isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common

isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common

isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common

isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common

isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common

isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common

isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common

isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common

isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common

extern fault_handler
isr_common:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret


SECTION .bss
	resb 8192		; reserve 4kb for our stack
_sys_stack:			; and set our stack pointer to the end
