/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/sizes.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/page.h>
#include <asm/cache.h>

#ifdef CONFIG_I386
#define LOAD_OFFSET 0xC0000000
#endif

/*
#define PAGE_SIZE 16384
#define RO_EXCEPTION_TABLE_ALIGN	4
*/

/*
 * Put .bss..swapper_pg_dir as the first thing in .bss. This will
 * ensure that it has .bss alignment (64K).
 */
#define BSS_FIRST_SECTIONS *(.bss..swapper_pg_dir)

#include <asm-generic/vmlinux.lds.h>
#include "image-vars.h"

#define BSS_DECRYPTED

/*
 * Max avaliable Page Size is 64K, so we set SectionAlignment
 * field of EFI application to 64K.
 */
PECOFF_FILE_ALIGN = 0x200;
PECOFF_SEGMENT_ALIGN = 0x10000;

#ifdef CONFIG_I386
OUTPUT_ARCH(i386)
#endif
ENTRY(start)
PHDRS {
	text PT_LOAD FLAGS(5);	/* RWX */
	note PT_NOTE FLAGS(6);	/* R__ */
}

/* jiffies	 = jiffies_64; */

SECTIONS
{
	. = VMLINUX_LOAD_ADDRESS;

	_text = .;
	.head.text : AT(ADDR(.head.text) - LOAD_OFFSET) {
		HEAD_TEXT
		. = ALIGN(4096);
	}

	. = LOAD_OFFSET;

	/* Text and read-only data */
	.text :  AT(ADDR(.text) - LOAD_OFFSET) {
		_text = .;
		_stext = .;
		/* bootstrapping code */
		HEAD_TEXT
		TEXT_TEXT
		SCHED_TEXT
		CPUIDLE_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		SOFTIRQENTRY_TEXT
		STATIC_CALL_TEXT
		ALIGN_ENTRY_TEXT_BEGIN
		ENTRY_TEXT
		ALIGN_ENTRY_TEXT_END
		*(.gnu.warning)

	} :text =0xcccc

	/* End of text section, which should occupy whole number of pages */
	_etext = .;
	. = ALIGN(PAGE_SIZE);

	/* Data */
	.data : AT(ADDR(.data) - LOAD_OFFSET) {
		/* Start of data section */
		_sdata = .;

		/* init_task */
		INIT_TASK_DATA(THREAD_SIZE)

		PAGE_ALIGNED_DATA(PAGE_SIZE)

		CACHELINE_ALIGNED_DATA(L1_CACHE_BYTES)

		DATA_DATA
		CONSTRUCTORS

		/* rarely changed data like cpu maps */
		READ_MOSTLY_DATA(INTERNODE_CACHE_BYTES)

		/* End of data section */
		_edata = .;
	} :data

	BUG_TABLE

	ORC_UNWIND_TABLE

	/* Init code and data - will be freed after init */
	. = ALIGN(PAGE_SIZE);
	.init.begin : AT(ADDR(.init.begin) - LOAD_OFFSET) {
		__init_begin = .; /* paired with __init_end */
	}

	INIT_TEXT_SECTION(PAGE_SIZE)

	INIT_DATA_SECTION(16)

	. = ALIGN(PAGE_SIZE);

	/* freed after init ends here */
	.init.end : AT(ADDR(.init.end) - LOAD_OFFSET) {
		__init_end = .;
	}

	/*
	 * smp_locks might be freed after init
	 * start/end must be page aligned
	 */
	. = ALIGN(PAGE_SIZE);
	.smp_locks : AT(ADDR(.smp_locks) - LOAD_OFFSET) {
		__smp_locks = .;
		*(.smp_locks)
		. = ALIGN(PAGE_SIZE);
		__smp_locks_end = .;
	}

	/* BSS */
	. = ALIGN(PAGE_SIZE);
	.bss : AT(ADDR(.bss) - LOAD_OFFSET) {
		__bss_start = .;
		*(.bss..page_aligned)
		. = ALIGN(PAGE_SIZE);
		*(BSS_MAIN)
		BSS_DECRYPTED
		. = ALIGN(PAGE_SIZE);
		__bss_stop = .;
	}

	/*
	 * The memory occupied from _text to here, __end_of_kernel_reserve, is
	 * automatically reserved in setup_arch(). Anything after here must be
	 * explicitly reserved using memblock_reserve() or it will be discarded
	 * and treated as available memory.
	 */
	__end_of_kernel_reserve = .;

	. = ALIGN(PAGE_SIZE);
	.brk : AT(ADDR(.brk) - LOAD_OFFSET) {
		__brk_base = .;
		. += 64 * 1024;		/* 64k alignment slop space */
		*(.bss..brk)		/* areas brk users have reserved */
		__brk_limit = .;
	}

	. = ALIGN(PAGE_SIZE);		/* keep VO_INIT_SIZE page aligned */
	_end = .;
}
