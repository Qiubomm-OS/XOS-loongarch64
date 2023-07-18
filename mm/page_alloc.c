#include <linux/mmzone.h>
#include <linux/debug.h>
#include <asm/page.h>

pg_data_t *pgdat_list;

/*
 * Set up the zone data structures:
 *   - mark all pages reserved
 *   - mark all memory queues empty
 *   - clear the memory bitmaps
 */
void free_area_init_core(int nid, pg_data_t *pgdat, struct page **gmap,
			unsigned long *zones_size, unsigned long zone_start_paddr,
			unsigned long *zholes_size, struct page *lmem_map)
{
	unsigned long i, j;
	unsigned long map_size;
	unsigned long totalpages, offset, realtotalpages;
	const unsigned long zone_required_alignment = 1UL << (MAX_ORDER-1);   // 这个区域必须邻近由伙伴分配器分配的最大大小的块，从而进行位级操作

	if (zone_start_paddr & ~PAGE_MASK)   // 如果物理地址不是按页面排列的，就是一个 BUG
		BUG();

	totalpages = 0;   // 为这个节点初始化 totalpages 为 0
	for (i = 0; i < MAX_NR_ZONES; i++) {   // 通过遍历 zone_sizes 来计算节点的总大小
		unsigned long size = zones_size[i];
		totalpages += size;
	}

	realtotalpages = totalpages;
	if (zholes_size)   // 通过减去 zholes_size 的空洞大小来计算实际的内存量
		for (i = 0; i < MAX_NR_ZONES; i++)
			realtotalpages -= zholes_size[i];
	
	printk("On node %d totalpages: %lu\n", nid, realtotalpages);   // 打印提示信息告知用户这个节点可用的内存量
}
