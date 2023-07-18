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
	struct list_head list;			/* ->mapping has some page lists. 指向链表中的下一页*/
	// struct address_space *mapping;	/* The inode (or ...) we belong to. 用来指定我们正在映射的索引节点（inode）*/
	unsigned long index;			/* Our offset within mapping. 在映射表中的偏移*/
	struct page *next_hash;			/* Next page sharing our hash bucket in the pagecache hash table. 指向页高速缓存哈希表中下一个共享的页*/
	atomic_t count;				/* Usage count, see below. 引用这个页的个数*/
	unsigned long flags;			/* atomic flags, some possibly updated asynchronously 页面各种不同的属性*/
	struct list_head lru;			/* Pageout list, eg. active_list; protected by pagemap_lru_lock !! 用在active_list中*/
	struct page **pprev_hash;		/* Complement to *next_hash. 与next_hash相对应*/
	// struct buffer_head * buffers;	/* Buffer maps us to a disk block. 把缓冲区映射到一个磁盘块*/
} mem_map_t;

#endif /* _XKERNEL_MM_H */
