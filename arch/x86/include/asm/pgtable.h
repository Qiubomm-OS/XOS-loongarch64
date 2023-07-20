#ifndef _I386_PGTABLE_H
#define _I386_PGTABLE_H

#include <asm/page.h>
#include <asm/pgtable-2level.h>
#include <asm/cpufeature.h>
#include <asm/processor.h>

#define pages_to_mb(x) ((x) >> (20-PAGE_SHIFT))

extern pgd_t swapper_pg_dir[1024] __attribute__((__aligned__(PAGE_SIZE)));
// extern pmd_t pmd0[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));   // 0 - 4M
// extern pmd_t pmd1[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));   // 4 - 8M

extern void paging_init(void);

/* Caches aren't brain-dead on the intel. */
#define flush_cache_all()						do { } while (0)
#define flush_cache_mm(mm)						do { } while (0)
#define flush_cache_range(mm, start, end)				do { } while (0)
#define flush_cache_page(vma, vmaddr)					do { } while (0)
#define flush_page_to_ram(page)						do { } while (0)
#define flush_dcache_page(page)						do { } while (0)
#define flush_icache_range(start, end)					do { } while (0)
#define flush_icache_page(vma,pg)					do { } while (0)
#define flush_icache_user_range(vma,pg,adr,len)	do { } while (0)

#define __flush_tlb()							\
	do {								\
		unsigned int tmpreg;					\
									\
		__asm__ __volatile__(					\
			"movl %%cr3, %0;  # flush TLB \n"		\
			"movl %0, %%cr3;              \n"		\
			: "=r" (tmpreg)					\
			:: "memory");					\
	} while (0)

/*
 * Global pages have to be flushed a bit differently. Not a real
 * performance problem because this does not happen often.
 */
#define __flush_tlb_global()							\
	do {												\
		unsigned int tmpreg;							\
														\
		__asm__ __volatile__(							\
			"movl %1, %%cr4;  # turn off PGE     \n"	\
			"movl %%cr3, %0;  # flush TLB        \n"	\
			"movl %0, %%cr3;                     \n"	\
			"movl %2, %%cr4;  # turn PGE back on \n"	\
			: "=&r" (tmpreg)							\
			: "r" (mmu_cr4_features & ~X86_CR4_PGE),	\
			  "r" (mmu_cr4_features)					\
			: "memory");								\
	} while (0)

extern unsigned long pgkern_mask;

/*
 * Do not check the PGE bit unnecesserily if this is a PPro+ kernel.
 */
#ifdef CONFIG_X86_PGE
# define __flush_tlb_all() __flush_tlb_global()
#else
# define __flush_tlb_all()						\
	do {								\
		if (cpu_has_pge)					\
			__flush_tlb_global();				\
		else							\
			__flush_tlb();					\
	} while (0)
#endif

#define __flush_tlb_single(addr) \
	__asm__ __volatile__("invlpg %0": :"m" (*(char *) addr))

#ifdef CONFIG_X86_INVLPG
# define __flush_tlb_one(addr) __flush_tlb_single(addr)
#else
# define __flush_tlb_one(addr)						\
	do {								\
		if (cpu_has_pge)					\
			__flush_tlb_single(addr);			\
		else							\
			__flush_tlb();					\
	} while (0)
#endif

/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern unsigned long empty_zero_page[1024];
#define ZERO_PAGE(vaddr) (virt_to_page(empty_zero_page))

// ==============================================================================

#define pgd_index(address) ((address >> PGDIR_SHIFT) & (PTRS_PER_PGD-1))
#define __pgd_offset(address) pgd_index(address)

/*
 * No page table caches to initialise
 */
#define pgtable_cache_init()	do { } while (0)

#define PMD_SIZE	(1UL << PMD_SHIFT)
// #define PMD_SIZE	0
#define PMD_MASK	(~(PMD_SIZE-1))
#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE-1))

/*
 * The 4MB page is guessing..  Detailed in the infamous "Chapter H"
 * of the Pentium details, but assuming intel did the straightforward
 * thing, this bit set in the page directory entry just means that
 * the page directory entry points directly to a 4MB-aligned block of
 * memory. 
 */
#define _PAGE_BIT_PRESENT	0
#define _PAGE_BIT_RW		1
#define _PAGE_BIT_USER		2
#define _PAGE_BIT_PWT		3
#define _PAGE_BIT_PCD		4
#define _PAGE_BIT_ACCESSED	5
#define _PAGE_BIT_DIRTY		6
#define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page, Pentium+, if present.. */
#define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */

#define _PAGE_PRESENT		0x001
#define _PAGE_RW		0x002
#define _PAGE_USER		0x004
#define _PAGE_PWT		0x008
#define _PAGE_PCD		0x010
#define _PAGE_ACCESSED		0x020
#define _PAGE_DIRTY		0x040
#define _PAGE_PSE		0x080	/* 4 MB (or 2MB) page, Pentium+, if present.. */
#define _PAGE_GLOBAL		0x100	/* Global TLB entry PPro+ */

#define _PAGE_PROTNONE		0x080	/* If not present */

#define _PAGE_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | _PAGE_ACCESSED | _PAGE_DIRTY)
#define _KERNPG_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_ACCESSED | _PAGE_DIRTY)
#define _PAGE_CHG_MASK	(PTE_MASK | _PAGE_ACCESSED | _PAGE_DIRTY)

#define PAGE_NONE	__pgprot(_PAGE_PROTNONE | _PAGE_ACCESSED)
#define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_COPY	__pgprot(_PAGE_PRESENT | _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_READONLY	__pgprot(_PAGE_PRESENT | _PAGE_USER | _PAGE_ACCESSED)

#define __PAGE_KERNEL \
	(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED)
#define __PAGE_KERNEL_NOCACHE \
	(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_PCD | _PAGE_ACCESSED)
#define __PAGE_KERNEL_RO \
	(_PAGE_PRESENT | _PAGE_DIRTY | _PAGE_ACCESSED)

#ifdef CONFIG_X86_PGE
# define MAKE_GLOBAL(x) __pgprot((x) | _PAGE_GLOBAL)
#else
# define MAKE_GLOBAL(x)						\
	({							\
		pgprot_t __ret;					\
		__ret = __pgprot(x);			\
		__ret;						\
	})
#endif

#define PAGE_KERNEL MAKE_GLOBAL(__PAGE_KERNEL)
#define PAGE_KERNEL_RO MAKE_GLOBAL(__PAGE_KERNEL_RO)
#define PAGE_KERNEL_NOCACHE MAKE_GLOBAL(__PAGE_KERNEL_NOCACHE)

/*
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */

#define mk_pte(page, pgprot)	__mk_pte((page) - mem_map, (pgprot))

/* This takes a physical page address that is used by the remapping functions */
#define mk_pte_phys(physpage, pgprot)	__mk_pte((physpage) >> PAGE_SHIFT, pgprot)


#define pmd_page(pmd) \
((unsigned long) __va(pmd_val(pmd) & PAGE_MASK))

/* Find an entry in the third-level page table.. */
#define __pte_offset(address) \
		((address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define pte_offset(dir, address) ((pte_t *) pmd_page(*(dir)) + \
			__pte_offset(address))

#endif /* _I386_PGTABLE_H */
