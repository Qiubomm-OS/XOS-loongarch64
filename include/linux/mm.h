#ifndef _XKERNEL_MM_H
#define _XKERNEL_MM_H

#include <linux/list.h>

/*
 * Each physical page in the system has a struct page associated with
 * it to keep track of whatever it is we are using the page for at the
 * moment. Note that we have no way to track which tasks are using
 * a page.
 *
 * Try to keep the most commonly accessed fields in single cache lines
 * here (16 bytes or greater).  This ordering should be particularly
 * beneficial on 32-bit processors.
 *
 * The first line is data used in page cache lookup, the second line
 * is used for linear searches (eg. clock algorithm scans). 
 *
 * TODO: make this structure smaller, it could be as small as 32 bytes.
 */
typedef struct page {
	/**
	 * ->mapping has some page lists.
	 * 指向链表中的下一页
	 */
	struct list_head list;
	/**
	 * The inode (or ...) we belong to.
	 * 用来指定我们正在映射的索引节点（inode
	 */
	// struct address_space *mapping;
	/**
	 * Our offset within mapping.
	 * 在映射表中的偏移
	 */
	unsigned long index;
	/**
	 * Next page sharing our hash bucket in the pagecache hash table.
	 * 指向页高速缓存哈希表中下一个共享的页
	 */
	struct page *next_hash;
	/**
	 * Usage count, see below.
	 * 引用这个页的个数
	 */
	atomic_t count;
	/**
	 * atomic flags, some possibly updated asynchronously
	 * 页面各种不同的属性
	 */
	unsigned long flags;
	/**
	 * Pageout list, eg. active_list; protected by pagemap_lru_lock !!
	 * 用在active_list中
	 */
	struct list_head lru;
	/**
	 * Complement to *next_hash.
	 * 与next_hash相对应
	 */
	struct page **pprev_hash;
	/**
	 * Buffer maps us to a disk block.
	 * 把缓冲区映射到一个磁盘块
	 */
	// struct buffer_head * buffers;
} mem_map_t;

extern mem_map_t * mem_map;
extern unsigned long max_mapnr;
extern unsigned long num_physpages;
extern unsigned long num_mappedpages;
extern void * high_memory;

#endif /* _XKERNEL_MM_H */
