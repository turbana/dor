
BASE	equ 0xF0000000		; base address of kernel

;.386P				; use 386+ privileged instuctions

;SECTION .text

;SECTION .text

;[BITS 16]

;;; MUTLTI BOOT HEADER
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MB_PAGE_ALIGN	equ 1<<0
MB_MEM_INFO	equ 1<<1
MB_AOUT_KLUDGE	equ 1<<16
MB_HEAD_MAGIC	equ 0x1BADB002
MB_HEAD_FLAGS	equ MB_PAGE_ALIGN | MB_MEM_INFO | MB_AOUT_KLUDGE
MB_CHECKSUM	equ -(MB_HEAD_MAGIC + MB_HEAD_FLAGS)

ALIGN 4				; make sure we are 4 byte alligned
extern code, bss, end
mboot:
	dd MB_HEAD_MAGIC	; multiboot header
	dd MB_HEAD_FLAGS
	dd MB_CHECKSUM

	dd mboot		; AOUT kludge - must be physical addresses
	dd code			; the linker will fill these in
	dd bss
	dd end
	dd entry16


;;; BASE KERNEL PAGE DIRECTORY / PAGE TABLE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;	ORG 0x3000

ALIGN 4096

PD:	times 1024 dd 0		; page directory
PT:	times 1024 dd 0		; page table


;;; ENTRY POINT (16 bit real mode)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;SECTION .text
;	ORG 0x5000
;ALIGN 4096

global entry16
entry16:

	;;; XXX what about loading the GDT here?

	mov eax, cr0		; activate protected mode
	or ax, 1
	mov cr0, eax
	jmp $+2			; flush instruction queue

	db 0x66
	db 0xEA
	dd entry32 - BASE
	dw 0x08


;;; ENTRY POINT (32 bit protected mode)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]

extern k_entry
entry32:
	mov esp, _sys_stack	; set up stack pointer

	mov eax, PD - BASE	; Store PT in PD[0] and PD[960]
	mov ebx, PT - BASE + 3
	mov [eax], ebx		; PD[0] = &PT
	mov eax, PD - BASE + 960 * 4
	mov [eax], ebx		; PD[960] = &PT

	mov edi, PT - BASE	; PT covers first 4mb physical memory
	mov eax, 3		; Addr (0) P (1) R/W (1)
	mov ecx, 1024		; loop 1024 times
initpt:	stosd
	add eax, 0x1000
	loop initpt

	mov eax, PD - BASE	; load PD
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000	; paging bit
	mov cr0, eax		; turn on paging
	jmp $+2			; flush instruction queue

	push k_entry
	ret

;	call k_entry		; jump to our entry point
	cli
	hlt


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


;;; IRQ ROUTINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
	cli			; disable interrupts
	push byte 0		; no error code
	push byte 32		; interrupts will start at 32
	jmp irq_common

irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common

irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common

irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common

irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common

irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common

irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common

irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common

irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common

irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common

irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common

irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common

irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common

irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common

irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common

irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common

extern irq_handler
irq_common:
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
	mov eax, irq_handler
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
