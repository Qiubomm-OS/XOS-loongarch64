#ifndef _LINUX_MMZONE_H
#define _LINUX_MMZONE_H

#include <linux/list.h>
#include <linux/types.h>
// #include <linux/spinlock.h>
// #include <linux/mm.h>

/*
 * Free memory management - zoned buddy allocator.
 */

#ifndef CONFIG_FORCE_MAX_ZONEORDER
#define MAX_ORDER 10
#else
#define MAX_ORDER CONFIG_FORCE_MAX_ZONEORDER
#endif

typedef struct free_area_struct {
	struct list_head	free_list;   //空闲页面块的链表
	unsigned long		*map;   //表示一对伙伴状态的位图
} free_area_t;

struct pglist_data;

/*
 * On machines where it is needed (eg PCs) we divide physical memory
 * into multiple physical zones. On a PC we have 3 zones:
 *
 * ZONE_DMA	  < 16 MB	ISA DMA capable memory
 * ZONE_NORMAL	16-896 MB	direct mapped by the kernel
 * ZONE_HIGHMEM	 > 896 MB	only page cache and user processes
 */
typedef struct zone_struct {
	/*
	 * Commonly accessed fields:
	 */
	// spinlock_t				lock;   	  // 并行访问时保护该管理区的自旋锁
	unsigned long				free_pages;   // 该管理区中空闲页面的总数
	unsigned long				pages_min, pages_low, pages_high;   // 管理区极值
	int					need_balance;   // 当可用页面的数量到达管理区极值的某一个值时，就需要平衡该管理区

	/*
	 * free areas of different sizes
	 */
	free_area_t				free_area[MAX_ORDER];   // 空闲区域位图，由伙伴分配器使用

	// wait_queue_head_t			* wait_table;   // 等待队列的哈希表，该队列由等待页面释放的进程组成。it's important for wait_on_page() and unlock_page()
	unsigned long				wait_table_size;    // 该哈希表的大小，是2的幂
	unsigned long				wait_table_shift;   // long型所对应的位数减去上述表大小的二进制对数。

	/*
	 * Discontig memory support fields.
	 */
	struct pglist_data			*zone_pgdat;   // 指向所属 pg_data_t
	struct page				*zone_mem_map;   // 涉及的管理区在全局 mem_map 中的第一页
	unsigned long				zone_start_paddr;   // 管理区的起始物理地址。更好的类型是PFN（页面帧号），一个PFN是一个简单的物理内存索引，以页面大小为基础的单位计算。PFN一般定义为 (page_phys_addr >> PAGE_SHIFT)
	unsigned long				zone_start_mapnr;   // 指出该管理区在全局 mem_map 中的页面偏移。在 free_area_init_core() 中，通过计算 mem_map 与该管理区的局部 mem_map 中称为 lmem_map 之间的页面数，从而得到页面偏移。

	/*
	 * rarely used fields:
	 */
	char					*name;   // 该管理区的字符串名字，“DMA”，“Normal”，“highMem”
	unsigned long				size;    // 该管理区的大小，以页面数计算
} zone_t;

#define ZONE_DMA		0
#define ZONE_NORMAL		1
#define ZONE_HIGHMEM	2
#define MAX_NR_ZONES	3

/*
 * One allocation request operates on a zonelist. A zonelist
 * is a list of zones, the first one is the 'goal' of the
 * allocation, the other zones are fallback zones, in decreasing
 * priority.
 *
 * Right now a zonelist takes up less than a cacheline. We never
 * modify it apart from boot-up, and only a few indices are used,
 * so despite the zonelist table being relatively big, the cache
 * footprint of this construct is very small.
 */
typedef struct zonelist_struct {
	zone_t * zones [MAX_NR_ZONES + 1];   // NULL delimited
} zonelist_t;

#define GFP_ZONEMASK	0x0f

/*
 * The pg_data_t structure is used in machines with CONFIG_DISCONTIGMEM
 * (mostly NUMA machines?) to denote a higher-level memory zone than the
 * zone_struct denotes.
 *
 * On NUMA machines, each NUMA node would have a pg_data_t to describe
 * it's memory layout.
 *
 * XXX: we need to move the global memory statistics (active_list, ...)
 *      into the pg_data_t to properly support NUMA.
 */
struct bootmem_data;
typedef struct pglist_data {
	zone_t node_zones[MAX_NR_ZONES];
	zonelist_t node_zonelists[GFP_ZONEMASK+1];
	int nr_zones;   // 表示该节点中管理区数目，1~3
	struct page *node_mem_map;   // 指 struct page 中第一个页面
	unsigned long *valid_addr_bitmap;
	struct bootmem_data *bdata;
	unsigned long node_start_paddr;   // 节点的起始物理地址
	unsigned long node_start_mapnr;   // 该节点在全局 mem_map 中的页面偏移
	unsigned long node_size;
	int node_id;   // 节点ID
	struct pglist_data *node_next;
} pg_data_t;

extern int numnodes;
extern pg_data_t *pgdat_list;

/*
 * The following two are not meant for general usage. They are here as
 * prototypes for the discontig memory code.
 */
// struct page;
// extern void show_free_areas_core(pg_data_t *pgdat);
// extern void free_area_init_core(int nid, pg_data_t *pgdat, struct page **gmap,
//   unsigned long *zones_size, unsigned long paddr, unsigned long *zholes_size,
//   struct page *pmap);

extern pg_data_t contig_page_data;

/**
 * for_each_pgdat - helper macro to iterate over all nodes
 * @pgdat - pg_data_t * variable
 *
 * Meant to help with common loops of the form
 * pgdat = pgdat_list;
 * while(pgdat) {
 * 	...
 * 	pgdat = pgdat->node_next;
 * }
 */
#define for_each_pgdat(pgdat) \
	for (pgdat = pgdat_list; pgdat; pgdat = pgdat->node_next)

/*
 * next_zone - helper magic for for_each_zone()
 * Thanks to William Lee Irwin III for this piece of ingenuity.
 */
static inline zone_t *next_zone(zone_t *zone)
{
	pg_data_t *pgdat = zone->zone_pgdat;

	if (zone - pgdat->node_zones < MAX_NR_ZONES - 1) {
		zone++;
	} else if (pgdat->node_next) {
		pgdat = pgdat->node_next;
		zone = pgdat->node_zones;
	} else {
		zone = (zone_t*)NULL;
	}

	return zone;
}

/**
 * for_each_zone - helper macro to iterate over all memory zones
 * @zone - zone_t * variable
 *
 * The user only needs to declare the zone variable, for_each_zone
 * fills it in. This basically means for_each_zone() is an
 * easier to read version of this piece of code:
 *
 * for(pgdat = pgdat_list; pgdat; pgdat = pgdat->node_next)
 * 	for(i = 0; i < MAX_NR_ZONES; ++i) {
 * 		zone_t * z = pgdat->node_zones + i;
 * 		...
 * 	}
 * }
 */
#define for_each_zone(zone) \
	for(zone = pgdat_list->node_zones; zone; zone = next_zone(zone))

#ifndef CONFIG_DISCONTIGMEM

#define NODE_DATA(nid)		(&contig_page_data)
#define NODE_MEM_MAP(nid)	mem_map
#define MAX_NR_NODES		1

#else /* !CONFIG_DISCONTIGMEM */

#include <asm/mmzone.h>

/* page->zone is currently 8 bits ... */
#ifndef MAX_NR_NODES
#define MAX_NR_NODES		(255 / MAX_NR_ZONES)
#endif

#endif /* !CONFIG_DISCONTIGMEM */

#define MAP_ALIGN(x)	((((x) % sizeof(mem_map_t)) == 0) ? (x) : ((x) + \
		sizeof(mem_map_t) - ((x) % sizeof(mem_map_t))))

#endif
