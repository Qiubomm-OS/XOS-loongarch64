#include <linux/bootmem.h>
#include <linux/mmzone.h>
#include <linux/stdio.h>
#include <linux/string.h>
#include <linux/debug.h>
#include <asm/bitops.h>
#include <asm/page.h>
#include <asm/io.h>

unsigned long max_low_pfn;
unsigned long min_low_pfn;
unsigned long max_pfn;

static void free_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;
	unsigned long start;
	/*
	 * round down end of usable mem, partially free pages are
	 * considered reserved.
	 * 向下取整可用内存的末端，部分空闲页保留
	 */
	unsigned long sidx;
	unsigned long eidx = (addr + size - bdata->node_boot_start) / PAGE_SIZE;
	unsigned long end = (addr + size) / PAGE_SIZE;

	if (!size) BUG();
	if (end > bdata->node_low_pfn) {
		BUG();
	}

	/*
	 * Round up the beginning of the address.
	 */
	start = (addr + PAGE_SIZE - 1) / PAGE_SIZE;
	sidx = start - (bdata->node_boot_start / PAGE_SIZE);

	for (i = sidx; i < eidx; i++) {
		if (!test_and_clear_bit(i, bdata->node_bootmem_map))
			BUG();
	}
}

/*
 * init_bootmem_core - Called once to set up the allocator itself.
 * 					   调用一次以设置分配器本身。
 * 返回值：位图大小（以字节为单位）
 */
static unsigned long init_bootmem_core (pg_data_t *pgdat,
	unsigned long mapstart, unsigned long start, unsigned long end)   // init_bootmem_core(&contig_page_data, start, 0, pages)
{
	bootmem_data_t *bdata = pgdat->bdata;   // pg_data_t contig_page_data = { bdata: &contig_bootmem_data };
	unsigned long mapsize = ((end - start + 1)+7)/8;   // 计算内存页位图的大小（以字节为单位），end = max_low_pfn，start = 0
	// printk("old mapsize = %d\n", mapsize);

	printk("bootmem_data_t init start.\n");

	pgdat->node_next = pgdat_list;   // 把 pgdat_data_t 插入到 pgdat_list 链表中
	pgdat_list = pgdat;

	mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);   // 保证是 4 的倍数
	// printk("now mapsize = %d\n", mapsize);
	bdata->node_bootmem_map = phys_to_virt(mapstart << PAGE_SHIFT);   // 初始化位图的起始地址，该位图用于表示页面的使用情况
	// printk("node_bootmem_map = %x\n", bdata->node_bootmem_map);
	bdata->node_boot_start = (start << PAGE_SHIFT);   // 记录该节点的起始地址
	bdata->node_low_pfn = end;   // 记录该节点的结束PFN

	/*
	 * Initially all pages are reserved - setup_arch() has to
	 * register free RAM areas explicitly.
	 */
	memset(bdata->node_bootmem_map, 0xff, mapsize);   // 初始化位图

	printk("bootmem_data_t init down.\n");
	return mapsize;
}

void free_bootmem(unsigned long addr, unsigned long size)
{
	return(free_bootmem_core(contig_page_data.bdata, addr, size));
}

unsigned long init_bootmem(unsigned long start, unsigned long pages)
{
	max_low_pfn = pages;
	min_low_pfn = start;
	return(init_bootmem_core(&contig_page_data, start, 0, pages));
}
