#ifndef __H_GDT
#define __H_GDT

/*
 * GDT Descriptor Layout
 * =====================
 *
 * The following diagram was taken from ``Intel(c) 64 and IA-32 Architectures
 * Software Developer's Manual Volume 3A: System Programming Guide, Part 1''.
 * The relevant information can be found in section ``3.4.5 Segment
 * Descriptors''. The full manual can be found at
 * <www.intel.com/design/processor/manuals/253668.pdf>.
 * 
 *      1                             1 0                             0
 *      F E D C B A 9 8 7 6 5 4 3 2 1 0 F E D C B A 9 8 7 6 5 4 3 2 1 0
 *     +-----------------+-+-+-+-------+-+---+-+-------+--------------+
 *     |               | |D| |A| SEG.  | | D | | TYPE  |              |
 *     |   BASE 31:24  |G|/|L|V| LIMIT |P| P |S| | | | |  BASE 23:16  |
 *     |               | |B| |L| 19:16 | | L | |1|2|3|4|              |
 *     +-----------------+-+-+-+-------+-+---+-+-------+--------------+
 * 
 *      1                             1 0                             0
 *      F E D C B A 9 8 7 6 5 4 3 2 1 0 F E D C B A 9 8 7 6 5 4 3 2 1 0
 *     +-------------------------------+------------------------------+
 *     |                               |                              |
 *     |      BASE ADDRESS 15:00       |      SEGMENT LIMIT 15:00     |
 *     |                               |                              |
 *     +-------------------------------+------------------------------+
 * 
 *  AVL    - Available for system software
 *  BASE   - Segment base address
 *  D/B    - Default operation size (0 = 16-bit segment; 1 = 32 bit)
 *  DPL    - Descriptor priviledge level
 *  G      - Granularity (0 = size in bytes; 1 = size in pages)
 *  L      - 64-bit code segment flag (0 = 32-bit; 1 = 64-bit code segment)
 *  LIMIT  - Segment limit (size of the segment, based on granularity)
 *  P      - Segment present (0 = no; 1 = yes)
 *  S      - Descriptor type (0 = system; 1 = code/data)
 *  TYPE 1 - Segment Type (0 = data; 1 = code)
 *  TYPE 2 - When data segment: Expand Direction (0 = up; 1 = down)
 *           When code segment: Conforming (0 = no; 1 = yes)
 *  TYPE 3 - When data segment: Write Access (0 = no; 1 = yes)
 *           When code segment: Execute Access (0 = no; 1 = yes)
 *  TYPE 4 - Segment Accessed (0 = no; 1 = yes)
 */

struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));		/* ensure GCC doesn't optimize this struct */

struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();		/* defined in boot.asm */

void gdt_set_gate(int, unsigned long, unsigned long, unsigned char,
				  unsigned char);
void gdt_install();

#endif
