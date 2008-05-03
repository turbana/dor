#ifndef __PAGING_H
#define __PAGING_H

/*
 * The following diagrams was taken from ``Intel(c) 64 and IA-32 Architectures
 * Software Developer's Manual Volume 3A: System Programming Guide, Part 1''.
 * The relevant information can be found in section ``3.7.6 Page-Directory and
 * Page-Table Entries''. The full manual can be found at
 * <www.intel.com/design/processor/manuals/253668.pdf>.
 *
 *
 * Page Directory Entry Layout
 * ===========================
 *
 *      1                             1 0                             0
 *      F E D C B A 9 8 7 6 5 4 3 2 1 0 F E D C B A 9 8 7 6 5 4 3 2 1 0
 *     +---------------------------------------+-----+-+-+-+-+-+-+-+-+-+
 *     |                                       |     | |P|A| |P|P|U|R| |
 *     |       PAGE TABLE BASE ADDRESS         |AVAIL|G|S|V|A|C|W|/|/|P|
 *     |                                       |     | | |L| |D|T|S|W| |
 *     +---------------------------------------+-----+-+-+-+-+-+-+-+-+-+
 *
 * Avail/AVL  - Available
 * A          - Accessed
 * G          - Global Page (ignored in PDEs)
 * P          - Present (0 = not present; 1 = present)
 * PS         - Page Size (0 = 4kb; 1 = 4mb)
 * PCD        - Page Cached Disabled (0 = cached; 1 = caching disabled)
 * PWT        - Page Write Through (0 = write back; 1 = write through)
 * R/W        - Read/Write (0 = read only; = 1 read/write)
 * U/S        - User/Supervisor (0 = supervisor; 1 = user)
 *
 *
 * Page Table Entry Diagram
 * ========================
 *
 *      1                             1 0                             0
 *      F E D C B A 9 8 7 6 5 4 3 2 1 0 F E D C B A 9 8 7 6 5 4 3 2 1 0
 *     +---------------------------------------+-----+-+-+-+-+-+-+-+-+-+
 *     |                                       |     | |P| | |P|P|U|R| |
 *     |           PAGE BASE ADDRESS           |AVAIL|G|A|D|A|C|W|/|/|P|
 *     |                                       |     | |T| | |D|T|S|W| |
 *     +---------------------------------------+-----+-+-+-+-+-+-+-+-+-+
 *
 * (Entries are the same as PDEs unless noted)
 * D          - Dirty
 * PAT        - Page Attribute Table Index (only pentium III+, otherwise
 *              reserved and should be 0)
 */

struct page_entry {
	unsigned int base_address;
	unsigned char flags;
} __attribute__((packed));

/* The following represent the possible values that a page directory or a page
 * table entry can have for it's flags.
 */
#define PAGE_LARGE		0x80	/* only for page directories */
#define PAGE_DIRTY		0x40	/* only for page tables */
#define PAGE_ACCESSED	0x20
#define PAGE_CACHED		0x10
#define PAGE_WT			0x08
#define PAGE_USER		0x04
#define PAGE_RW			0x02
#define PAGE_PRESENT	0x01

#define VIRT_TO_PHYS(loc) ((void *)loc + 0x40000000)

void write_page_entry(unsigned int *, struct page_entry *);
void paging_init();

#endif
