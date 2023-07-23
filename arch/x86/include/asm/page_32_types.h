#ifndef _ASM_PAGE_32_TYPES_H
#define _ASM_PAGE_32_TYPES_H

#include <linux/const.h>
#include <asm/page_types.h>

/*
 * This handles the memory map.
 *
 * A __PAGE_OFFSET of 0xC0000000 means that the kernel has
 * a virtual address space of one gigabyte, which limits the
 * amount of physical memory you can use to about 950MB.
 *
 * If you want more physical memory than this then see the CONFIG_HIGHMEM4G
 * and CONFIG_HIGHMEM64G options in the kernel configuration.
 */
/* #define __PAGE_OFFSET_BASE	_AC(CONFIG_PAGE_OFFSET, UL) */
#define __PAGE_OFFSET		(0xC0000000)

#define __START_KERNEL_map	__PAGE_OFFSET

#define THREAD_SIZE_ORDER	1
#define THREAD_SIZE		(PAGE_SIZE << THREAD_SIZE_ORDER)

#endif /* _ASM_PAGE_32_TYPES_H */
