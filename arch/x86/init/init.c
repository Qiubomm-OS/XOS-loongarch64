#include <linux/bootmem.h>
#include <linux/stdio.h>
#include <linux/debug.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/processor.h>

unsigned long highstart_pfn, highend_pfn;
static unsigned long totalram_pages;
static unsigned long totalhigh_pages;

pgd_t swapper_pg_dir[1024] __attribute__((__aligned__(PAGE_SIZE)));

static void pagetable_init(void)
{
	unsigned long vaddr, end;
	pgd_t *pgd, *pgd_base;
	int i, j, k;
	pmd_t *pmd;
	pte_t *pte, *pte_base;

	printk("pagetable_init start.\n");

	// printk("swapper_pg_dir = %x\n", swapper_pg_dir);

	/*
	 * This can be zero as well - no problem, in that case we exit
	 * the loops anyway due to thse PTRS_PER_* conditions.
	 */
	end = (unsigned long)__va(max_low_pfn*PAGE_SIZE);
	pgd_base = swapper_pg_dir;
	i = __pgd_offset(PAGE_OFFSET);   // PAGE_OFFSET = 0xC0000000，获得地址为 0xC0000000 在 PGD 中的便宜，i=0xc00
	pgd = pgd_base + i;

	// printk("swapper_pg_dir = %x\n", swapper_pg_dir);
	for (; i < PTRS_PER_PGD; pgd++, i++) {
		vaddr = i * PGDIR_SIZE;   // 获得虚拟地址 PDG中 索引为 i 的虚拟地址, vaddr = 0xC0000000
		if (end && (vaddr >= end)) {
				break;
			}
			pmd = (pmd_t *)pgd;   // 没有中间页目录，就把中间页目录直接映射到总目录
			if (pmd != pmd_offset(pgd, 0))
				BUG();
			
		for (j = 0; j < PTRS_PER_PMD; pmd++, j++) {
			vaddr = i*PGDIR_SIZE + j*PMD_SIZE;   // vaddr = 0xC0000000
			if (end && (vaddr >= end))
				break;
			pte_base = pte = (pte_t *) alloc_bootmem_low_pages(PAGE_SIZE);   // 设置中间页目录项，为一个页表（4K）分配空间
					// printk("pte_base = %x\n", pte_base);

			for (k = 0; k < PTRS_PER_PTE; pte++, k++) {
				vaddr = i*PGDIR_SIZE + j*PMD_SIZE + k*PAGE_SIZE;   // vaddr = 0xC0000000
				if (end && (vaddr >= end))
				break;
				*pte = mk_pte_phys(__pa(vaddr), PAGE_KERNEL);   // 宏mk_pte_phys()创建一个页表项，这个页表项的物理地址为__pa(vaddr)。属性PAGE_KERNEL表示只有在内核态才能访问这一页表项
			}
			set_pmd(pmd, __pmd(_KERNPG_TABLE + __pa(pte_base)));   // 通过调用set_pmd()把该页表追加到中间页目录中。这个过程一直继续，直到把所有的物理内存都映射到从PAGE_OFFSET开始的虚拟地址空间			// printk("%x\n", pte_offset(pmd, 0));
					// printk("pte_offset = %x\n", pte_offset(pmd, 0));
			if (pte_base != pte_offset(pmd, 0)) {
				BUG();
			}
		}
	}

	printk("pagetable_init down.\n");
}

static void zone_sizes_init(void)
{
	unsigned long zones_size[MAX_NR_ZONES] = {0, 0, 0};
	unsigned int max_dma, high, low;

	/**
	 * 低于 16MB 的内存只能用于 DMA，因此，这条语句用于存放16MB的页面, MAX_DMA_ADDRESS = 0xC1000000, max_dma = 0x1000
	 */
	max_dma = virt_to_phys((char*)MAX_DMA_ADDRESS) >> PAGE_SHIFT;
	low = max_low_pfn;

	if(low < max_dma) {
		zones_size[ZONE_DMA] = low;
	} else {
		zones_size[ZONE_DMA] = max_dma;
		zones_size[ZONE_NORMAL] = low - max_dma;
	}
}
