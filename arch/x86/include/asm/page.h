#ifndef _ASM_PAGE_H
#define _ASM_PAGE_H

#ifdef CONFIG_X86_64
// #include <asm/page_64.h>
#else
#include <asm/page_32.h>
#endif	/* CONFIG_X86_64 */

#ifndef __ASSEMBLY__

#define clear_page(page) memset((void*)(page), 0, PAGE_SIZE)
#define copy_page(to, from) memcpy((void*)(to), (void*)(from), PAGE_SIZE)

#define PAGE_OFFSET		((unsigned long)__PAGE_OFFSET)

/**
 * P--位0是存在（Present）标志，用于指明表项对地址转换是否有效。P=1表示有效；P=0表示无效。
 * 在页转换过程中，如果说涉及的页目录或页表的表项无效，则会导致一个异常。
 * 如果P=0，那么除表示表项无效外，其余位可供程序自由使用。
 * 例如，操作系统可以使用这些位来保存已存储在磁盘上的页面的序号。
 */
#define PAGE_PRESENT 	0x1
#define PAGE_USER 0x4
/**
 * R/W--位1是读/写（Read/Write）标志。如果等于1，表示页面可以被读、写或执行。
 * 如果为0，表示页面只读或可执行。
 * 当处理器运行在超级用户特权级（级别0、1或2）时，则R/W位不起作用。
 * 页目录项中的R/W位对其所映射的所有页面起作用。
 */
#define PAGE_WRITE 	0x2


#define __pa(x)			((unsigned long)(x)-PAGE_OFFSET)
#define __va(x)			((void *)((unsigned long)(x)+PAGE_OFFSET))

/*
 * This much address space is reserved for vmalloc() and iomap()
 * as well as fixmap mappings.
 */
#define __VMALLOC_RESERVE	(128 << 20)
#define VMALLOC_RESERVE		((unsigned long)__VMALLOC_RESERVE)
#define __MAXMEM		(-__PAGE_OFFSET-__VMALLOC_RESERVE)
#define MAXMEM			((unsigned long)(-PAGE_OFFSET-VMALLOC_RESERVE))

typedef struct { unsigned long pte_low; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;
#define pte_val(x)	((x).pte_low)
#define pmd_val(x)	((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x) ((pte_t) { (x) } )
#define __pmd(x) ((pmd_t) { (x) } )
#define __pgd(x) ((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )
#define VALID_PAGE(page)	((page - mem_map) < max_mapnr)
#define virt_to_page(kaddr)	(mem_map + (__pa(kaddr) >> PAGE_SHIFT))

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_PAGE_H */
