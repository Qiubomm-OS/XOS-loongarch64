#ifndef _XKERNEL_MM_H
#define _XKERNEL_MM_H

#include <linux/list.h>
#include <linux/mmzone.h>
#include <asm/bitsperlong.h>
#include <asm/atomic.h>

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

/*
 * Methods to modify the page usage count.
 *
 * What counts for a page usage:
 * - cache mapping   (page->mapping)
 * - disk mapping    (page->buffers)
 * - page mapped in a task's page tables, each mapping
 *   is counted separately
 *
 * Also, many kernel routines increase the page count before a critical
 * routine so they can be sure the page doesn't go away from under them.
 */
#define get_page(p)		        arch_atomic_inc(&(p)->count)
// #define put_page(p)		        __free_page(p)
// #define put_page_testzero(p) 		atomic_dec_and_test(&(p)->count)
#define page_count(p)		    	arch_atomic_read(&(p)->count)
#define set_page_count(p,v) 		arch_atomic_set(&(p)->count, v)

/*
 * Various page->flags bits:
 *
 * PG_reserved is set for special pages, which can never be swapped
 * out. Some of them might not even exist (eg empty_bad_page)...
 *
 * Multiple processes may "see" the same page. E.g. for untouched
 * mappings of /dev/null, all processes see the same page full of
 * zeroes, and text pages of executables and shared libraries have
 * only one copy in memory, at most, normally.
 *
 * For the non-reserved pages, page->count denotes a reference count.
 *   page->count == 0 means the page is free.
 *   page->count == 1 means the page is used for exactly one purpose
 *   (e.g. a private data page of one process).
 *
 * A page may be used for kmalloc() or anyone else who does a
 * __get_free_page(). In this case the page->count is at least 1, and
 * all other fields are unused but should be 0 or NULL. The
 * management of this page is the responsibility of the one who uses
 * it.
 *
 * The other pages (we may call them "process pages") are completely
 * managed by the Linux memory manager: I/O, buffers, swapping etc.
 * The following discussion applies only to them.
 *
 * A page may belong to an inode's memory mapping. In this case,
 * page->mapping is the pointer to the inode, and page->index is the
 * file offset of the page, in units of PAGE_CACHE_SIZE.
 *
 * A page may have buffers allocated to it. In this case,
 * page->buffers is a circular list of these buffer heads. Else,
 * page->buffers == NULL.
 *
 * For pages belonging to inodes, the page->count is the number of
 * attaches, plus 1 if buffers are allocated to the page, plus one
 * for the page cache itself.
 *
 * All pages belonging to an inode are in these doubly linked lists:
 * mapping->clean_pages, mapping->dirty_pages and mapping->locked_pages;
 * using the page->list list_head. These fields are also used for
 * freelist managemet (when page->count==0).
 *
 * There is also a hash table mapping (mapping,index) to the page
 * in memory if present. The lists for this hash table use the fields
 * page->next_hash and page->pprev_hash.
 *
 * All process pages can do I/O:
 * - inode pages may need to be read from disk,
 * - inode pages which have been modified and are MAP_SHARED may need
 *   to be written to disk,
 * - private pages which have been modified may need to be swapped out
 *   to swap space and (later) to be read back into memory.
 * During disk I/O, PG_locked is used. This bit is set before I/O
 * and reset when I/O completes. page_waitqueue(page) is a wait queue of all
 * tasks waiting for the I/O on this page to complete.
 * PG_uptodate tells whether the page's contents is valid.
 * When a read completes, the page becomes uptodate, unless a disk I/O
 * error happened.
 *
 * For choosing which pages to swap out, inode pages carry a
 * PG_referenced bit, which is set any time the system accesses
 * that page through the (mapping,index) hash table. This referenced
 * bit, together with the referenced bit in the page tables, is used
 * to manipulate page->age and move the page across the active,
 * inactive_dirty and inactive_clean lists.
 *
 * Note that the referenced bit, the page->lru list_head and the
 * active, inactive_dirty and inactive_clean lists are protected by
 * the pagemap_lru_lock, and *NOT* by the usual PG_locked bit!
 *
 * PG_skip is used on sparc/sparc64 architectures to "skip" certain
 * parts of the address space.
 *
 * PG_error is set to indicate that an I/O error occurred on this page.
 *
 * PG_arch_1 is an architecture specific page state bit.  The generic
 * code guarantees that this bit is cleared for a page when it first
 * is entered into the page cache.
 *
 * PG_highmem pages are not permanently mapped into the kernel virtual
 * address space, they need to be kmapped separately for doing IO on
 * the pages. The struct page (these bits with information) are always
 * mapped into kernel address space...
 */
#define PG_locked		0	/* Page is locked. Don't touch. */
#define PG_error		1
#define PG_referenced		2
#define PG_uptodate		3
#define PG_dirty		4
#define PG_unused		5
#define PG_lru			6
#define PG_active		7
#define PG_slab			8
#define PG_skip			10
#define PG_highmem		11
#define PG_checked		12	/* kill me in 2.5.<early>. */
#define PG_arch_1		13
#define PG_reserved		14
#define PG_launder		15	/* written out by VM pressure.. */
#define PG_fs_1			16	/* Filesystem specific */

#ifndef arch_set_page_uptodate
#define arch_set_page_uptodate(page)
#endif

/* Make it prettier to test the above... */
#define UnlockPage(page)	unlock_page(page)
#define Page_Uptodate(page)	test_bit(PG_uptodate, &(page)->flags)
#define SetPageUptodate(page) \
	do {								\
		arch_set_page_uptodate(page);				\
		set_bit(PG_uptodate, &(page)->flags);			\
	} while (0)
#define ClearPageUptodate(page)	clear_bit(PG_uptodate, &(page)->flags)
#define PageDirty(page)		test_bit(PG_dirty, &(page)->flags)
#define SetPageDirty(page)	set_bit(PG_dirty, &(page)->flags)
#define ClearPageDirty(page)	clear_bit(PG_dirty, &(page)->flags)
#define PageLocked(page)	test_bit(PG_locked, &(page)->flags)
#define LockPage(page)		set_bit(PG_locked, &(page)->flags)
#define TryLockPage(page)	test_and_set_bit(PG_locked, &(page)->flags)
#define PageChecked(page)	test_bit(PG_checked, &(page)->flags)
#define SetPageChecked(page)	set_bit(PG_checked, &(page)->flags)
#define PageLaunder(page)	test_bit(PG_launder, &(page)->flags)
#define SetPageLaunder(page)	set_bit(PG_launder, &(page)->flags)
#define ClearPageLaunder(page)	clear_bit(PG_launder, &(page)->flags)

/*
 * The zone field is never updated after free_area_init_core()
 * sets it, so none of the operations on it need to be atomic.
 */
#define NODE_SHIFT 4
#define ZONE_SHIFT (BITS_PER_LONG - 8)

// struct zone_struct;
extern struct zone_struct *zone_table[];

static inline zone_t *page_zone(struct page *page)
{
	return zone_table[page->flags >> ZONE_SHIFT];
}

static inline void set_page_zone(struct page *page, unsigned long zone_num)
{
	page->flags &= ~(~0UL << ZONE_SHIFT);
	page->flags |= zone_num << ZONE_SHIFT;
}

/*
 * In order to avoid #ifdefs within C code itself, we define
 * set_page_address to a noop for non-highmem machines, where
 * the field isn't useful.
 * The same is true for page_address() in arch-dependent code.
 */
#if defined(CONFIG_HIGHMEM) || defined(WANT_PAGE_VIRTUAL)

#define set_page_address(page, address)			\
	do {						\
		(page)->virtual = (address);		\
	} while(0)

#else /* CONFIG_HIGHMEM || WANT_PAGE_VIRTUAL */
#define set_page_address(page, address)  do { } while(0)
#endif /* CONFIG_HIGHMEM || WANT_PAGE_VIRTUAL */

/*
 * Permanent address of a page. Obviously must never be
 * called on a highmem page.
 */
#if defined(CONFIG_HIGHMEM) || defined(WANT_PAGE_VIRTUAL)

#define page_address(page) ((page)->virtual)

#else /* CONFIG_HIGHMEM || WANT_PAGE_VIRTUAL */

#define page_address(page)						\
	__va( (((page) - page_zone(page)->zone_mem_map) << PAGE_SHIFT)	\
			+ page_zone(page)->zone_start_paddr)

#endif /* CONFIG_HIGHMEM || WANT_PAGE_VIRTUAL */

/*
 * The first mb is necessary to safely close the critical section opened by the
 * TryLockPage(), the second mb is necessary to enforce ordering between
 * the clear_bit and the read of the waitqueue (to avoid SMP races with a
 * parallel wait_on_page).
 */
#define PageError(page)		        	test_bit(PG_error, &(page)->flags)
#define SetPageError(page)	        	set_bit(PG_error, &(page)->flags)
#define ClearPageError(page)	    		clear_bit(PG_error, &(page)->flags)
#define PageReferenced(page)	    		test_bit(PG_referenced, &(page)->flags)
#define SetPageReferenced(page)	    		set_bit(PG_referenced, &(page)->flags)
#define ClearPageReferenced(page)		clear_bit(PG_referenced, &(page)->flags)
#define PageTestandClearReferenced(page)	test_and_clear_bit(PG_referenced, &(page)->flags)
#define PageSlab(page)		    		test_bit(PG_slab, &(page)->flags)
#define PageSetSlab(page)	    		set_bit(PG_slab, &(page)->flags)
#define PageClearSlab(page)	    		clear_bit(PG_slab, &(page)->flags)
#define PageReserved(page)	    		test_bit(PG_reserved, &(page)->flags)

#define PageActive(page)	    		test_bit(PG_active, &(page)->flags)
#define SetPageActive(page)	    		set_bit(PG_active, &(page)->flags)
#define ClearPageActive(page)			clear_bit(PG_active, &(page)->flags)

#define PageLRU(page)		    		test_bit(PG_lru, &(page)->flags)
#define TestSetPageLRU(page)			test_and_set_bit(PG_lru, &(page)->flags)
#define TestClearPageLRU(page)			test_and_clear_bit(PG_lru, &(page)->flags)

#ifdef CONFIG_HIGHMEM
#define PageHighMem(page)			test_bit(PG_highmem, &(page)->flags)
#else
#define PageHighMem(page)			0 /* needed to optimize away at compile time */
#endif

#define SetPageReserved(page)			set_bit(PG_reserved, &(page)->flags)
#define ClearPageReserved(page)			clear_bit(PG_reserved, &(page)->flags)

/*
 * Error return values for the *_nopage functions
 */
#define NOPAGE_SIGBUS	(NULL)
#define NOPAGE_OOM	((struct page *) (-1))

extern mem_map_t * mem_map;
extern unsigned long max_mapnr;
extern unsigned long num_physpages;
extern unsigned long num_mappedpages;
extern void * high_memory;

void free_area_init(unsigned long *zones_size);

/*
 * GFP bitmasks..
 */
/* Zone modifiers in GFP_ZONEMASK (see linux/mmzone.h - low four bits) */
#define __GFP_DMA		0x01
#define __GFP_HIGHMEM		0x02

/* Action modifiers - doesn't change the zoning */
#define __GFP_WAIT		0x10	/* Can wait and reschedule? */
#define __GFP_HIGH		0x20	/* Should access emergency pools? */
#define __GFP_IO		0x40	/* Can start low memory physical IO? */
#define __GFP_HIGHIO		0x80	/* Can start high mem physical IO? */
#define __GFP_FS		0x100	/* Can call down to low-level FS? */

#define GFP_NOHIGHIO		(__GFP_HIGH | __GFP_WAIT | __GFP_IO)
#define GFP_NOIO		(__GFP_HIGH | __GFP_WAIT)
#define GFP_NOFS		(__GFP_HIGH | __GFP_WAIT | __GFP_IO | __GFP_HIGHIO)
#define GFP_ATOMIC		(__GFP_HIGH)
#define GFP_USER		(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_HIGHUSER		(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS | __GFP_HIGHMEM)
#define GFP_KERNEL		(__GFP_HIGH | __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_NFS			(__GFP_HIGH | __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)
#define GFP_KSWAPD		(             __GFP_WAIT | __GFP_IO | __GFP_HIGHIO | __GFP_FS)

/* Flag - indicates that the buffer will be suitable for DMA.  Ignored on some
   platforms, used as appropriate on others */

#define GFP_DMA			__GFP_DMA

#define __get_free_page(gfp_mask) \
		__get_free_pages((gfp_mask),0)

#endif /* _XKERNEL_MM_H */
