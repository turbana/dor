#ifndef __H_GDT
#define __H_GDT

/*
 * Global Descriptor Table
 * =======================
 *
 * The GDT is used by the CPU to map segments onto physical memory. The concept
 * is nearly the same as for paging. Either the CS (code segment), DS (data
 * segment), SS (stack segment) or another segment (ES, FS, GS) is loaded with
 * a GDT descriptor entry. When the CPU goes to make a memory fetch it looks
 * at the current segment register's hidden cache and checks that the logical
 * address falls within the segment's limit. If it does not a GPF is raised,
 * otherwise the logical address is added to the segment's base address to form
 * the physical memory address.
 *
 * Because the GDT entries, once loaded, are actually stored in hidden register
 * caches, simply modifying the GDT *will not* update any memory fetches. We
 * first update the GDT, and then update all the segment registers. See
 * ``boot.asm'' for an example of this.
 *
 * Most of the time we set all GDT entries and segment registers to have a
 * base of 0 and a limit of 0xFFFFFFFF. This effectively turns off
 * segmentation as each memory fetch is checked to see if it falls within the
 * 4gb limit and then has 0 added to it. An expensive noop, but that is the
 * only way to effectively disable segmentation.
 *
 * The above isn't entirely true though. For a brief time during kernel loading
 * (namely before paging is turned on) we set each GDT entry to have a base
 * address of 0x40000000. The reason we do this is we want the kernel loaded
 * into high memory (0xC0000000), but the bootloader can only load us into low
 * memory (0x00100000). By setting the segments to have a base of 0x40000000 it
 * causes each memory fetch to have 0x40000000 added to it. This causes integer
 * overflow when added to our high addresses, wrapping them around into
 * low memory. This lets us execute instructions in high virtual memory while
 * being loaded into low physical memory and still have paging disabled. Of
 * course once paging is turned on the GDT is flushed and the entries set back
 * to 0. 
 *
 * Having the initial GDT initialized to 0x40000000 forces us to translate
 * certain addresses by hand until paging is turned on. This is only a concern
 * to those pointers who need to be stored *before* paging, but not followed
 * until *after* paging is enabled. In those cases we simply add 0x40000000 to
 * the pointer by hand. This is needed, of course, in ``paging.c''.
 *
 *
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
 *     +-----------------+-+-+-+-------+-+---+-+-------+---------------+
 *     |               | |D| |A| SEG.  | | D | | TYPE  |               |
 *     |   BASE 31:24  |G|/|L|V| LIMIT |P| P |S| | | | |   BASE 23:16  |
 *     |               | |B| |L| 19:16 | | L | |1|2|3|4|               |
 *     +-----------------+-+-+-+-------+-+---+-+-------+---------------+
 * 
 *      1                             1 0                             0
 *      F E D C B A 9 8 7 6 5 4 3 2 1 0 F E D C B A 9 8 7 6 5 4 3 2 1 0
 *     +-------------------------------+-------------------------------+
 *     |                               |                               |
 *     |      BASE ADDRESS 15:00       |      SEGMENT LIMIT 15:00      |
 *     |                               |                               |
 *     +-------------------------------+-------------------------------+
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
void gdt_init();

#endif
