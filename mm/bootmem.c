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

/*
 * init_bootmem_core - Called once to set up the allocator itself.
 * 					   调用一次以设置分配器本身。
 * 返回值：位图大小（以字节为单位）
 */
static unsigned long init_bootmem_core(pg_data_t *pgdat,
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

static void reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;

	/*
	 * round up, partially reserved pages are considered
	 * fully reserved.
	 */
	unsigned long sidx = (addr - bdata->node_boot_start)/PAGE_SIZE;
	unsigned long eidx = (addr + size - bdata->node_boot_start + 
							PAGE_SIZE-1)/PAGE_SIZE;
	unsigned long end = (addr + size + PAGE_SIZE-1)/PAGE_SIZE;

	if (!size) BUG();

	if (sidx < 0) { BUG(); }
	if (eidx < 0) { BUG(); }
	if (sidx >= eidx) { BUG(); }
	if ((addr >> PAGE_SHIFT) >= bdata->node_low_pfn) { BUG(); }
	if (end > bdata->node_low_pfn) { BUG(); }
	for (i = sidx; i < eidx; i++) {
		if (test_and_set_bit(i, bdata->node_bootmem_map)) {
			printk("hm, page %08lx reserved twice.\n", i * PAGE_SIZE);
		}
	}
}

/*
 * We 'merge' subsequent allocations to save space. We might 'lose'
 * some fraction of a page if allocations cannot be satisfied due to
 * size constraints on boxes where there is physical RAM space
 * fragmentation - in these cases * (mostly large memory boxes) this
 * is not a problem.
 *
 * On low memory boxes we get it right in 100% of the cases.
 */

/*
 * alignment has to be a power of 2 value.
 */
static void * __alloc_bootmem_core(bootmem_data_t *bdata, 
	unsigned long size, unsigned long align, unsigned long goal)
{
	unsigned long i, start = 0;
	void *ret;
	unsigned long offset, remaining_size;
	unsigned long areasize, preferred, incr;
	unsigned long eidx = bdata->node_low_pfn - (bdata->node_boot_start >>
							PAGE_SHIFT);

	if (!size) BUG();

	if (align & (align-1))
		BUG();

	offset = 0;
	if (align && (bdata->node_boot_start & (align - 1UL)) != 0) {
		offset = (align - (bdata->node_boot_start & (align - 1UL)));
	}
	offset >>= PAGE_SHIFT;

	/*
	 * We try to allocate bootmem pages above 'goal'
	 * 我们尝试在 goal 上方分配 bootmem 页面
	 * first, then we try to allocate lower pages.
	 * 首先，然后我们尝试分配较低的页面
	 */
	if (goal && (goal >= bdata->node_boot_start) && ((goal >> PAGE_SHIFT) < bdata->node_low_pfn)) {
		preferred = goal - bdata->node_boot_start;
	} else {
		preferred = 0;
	}

	preferred = ((preferred + align - 1) & ~(align - 1)) >> PAGE_SHIFT;
	preferred += offset;
	areasize = (size+PAGE_SIZE-1)/PAGE_SIZE;
	incr = align >> PAGE_SHIFT ? : 1;

restart_scan:
	for (i = preferred; i < eidx; i += incr) {
		unsigned long j;
		if (test_bit(i, bdata->node_bootmem_map))
			continue;
		for (j = i + 1; j < i + areasize; ++j) {
			if (j >= eidx)
				goto fail_block;
			if (test_bit (j, bdata->node_bootmem_map))
				goto fail_block;
		}
		start = i;
		goto found;
	fail_block:;
	}
	if (preferred) {
		preferred = offset;
		goto restart_scan;
	}
	return NULL;
found:
	if (start >= eidx)
		BUG();

	/*
	 * Is the next page of the previous allocation-end the start
	 * of this allocation's buffer? If yes then we can 'merge'
	 * the previous partial page with this allocation.
	 */
	if (align <= PAGE_SIZE
	    && bdata->last_offset && bdata->last_pos+1 == start) {
		offset = (bdata->last_offset+align-1) & ~(align-1);
		if (offset > PAGE_SIZE)
			BUG();
		remaining_size = PAGE_SIZE-offset;
		if (size < remaining_size) {
			areasize = 0;
			// last_pos unchanged
			bdata->last_offset = offset+size;
			ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
						bdata->node_boot_start);
		} else {
			remaining_size = size - remaining_size;
			areasize = (remaining_size+PAGE_SIZE-1)/PAGE_SIZE;
			ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
						bdata->node_boot_start);
			bdata->last_pos = start+areasize-1;
			bdata->last_offset = remaining_size;
		}
		bdata->last_offset &= ~PAGE_MASK;
	} else {
		bdata->last_pos = start + areasize - 1;
		bdata->last_offset = size & ~PAGE_MASK;
		ret = phys_to_virt(start * PAGE_SIZE + bdata->node_boot_start);
	}
	/*
	 * Reserve the area now:
	 */
	for (i = start; i < start+areasize; i++)
		if (test_and_set_bit(i, bdata->node_bootmem_map))
			BUG();
	memset(ret, 0, size);
	return ret;
}

unsigned long init_bootmem(unsigned long start, unsigned long pages)
{
	max_low_pfn = pages;
	min_low_pfn = start;
	return(init_bootmem_core(&contig_page_data, start, 0, pages));
}

void free_bootmem(unsigned long addr, unsigned long size)
{
	return(free_bootmem_core(contig_page_data.bdata, addr, size));
}

void reserve_bootmem(unsigned long addr, unsigned long size)
{
	return(reserve_bootmem_core(contig_page_data.bdata, addr, size));
}

void * __alloc_bootmem(unsigned long size, unsigned long align, unsigned long goal)
{
	pg_data_t *pgdat;
	void *ptr;

	for_each_pgdat(pgdat) {
		if ((ptr = __alloc_bootmem_core(pgdat->bdata, size, align, goal))) {
			return(ptr);
		}
	}

	/*
	 * Whoops, we cannot satisfy the allocation request.
	 */
	printk("bootmem alloc of %lu bytes failed!\n", size);
	// panic("Out of memory");
	BUG();
	return NULL;
}

void * __alloc_bootmem_node(pg_data_t *pgdat, unsigned long size, unsigned long align, unsigned long goal)
{
	void *ptr;

	ptr = __alloc_bootmem_core(pgdat->bdata, size, align, goal);
	if (ptr)
		return (ptr);

	printk("bootmem alloc of %lu bytes failed!\n", size);
	BUG();
	return NULL;
}
