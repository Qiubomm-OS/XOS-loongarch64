#include <linux/init.h>
#include <linux/types.h>
#include <asm/gdt.h>
#include <asm/multiboot.h>
#include <asm/page.h>
#include <asm/print.h>
#include <asm/pgtable.h>

// 内核栈的栈顶
uint32_t kern_stack_top;
void* glb_mboot_ptr;
extern void start_kernel(void);
extern void *flush;

#define PAGE_DIR_TABLE_POS  0x90000

// 内核使用的临时页表和页目录
// 该地址必须是页对齐的地址，内存 0-640KB 肯定是空闲的

// pgd_t swapper_pg_dir[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));
// pte_t pte0[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));   // 0 - 4M
// pte_t pte1[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));   // 4 - 8M

pgd_t pgd[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".init.data")));
pte_t pte[1024] __attribute__ ((__section__ (".init.data")));

unsigned long empty_zero_page[1024];

// pgd_t pgd[1024];
// pte_t pte[1024];

/* 创建gdt描述符 */
static struct gdt_desc __init make_gdt_desc(uint32_t* desc_addr, uint32_t limit, uint8_t attr_low, uint8_t attr_high) {
	uint32_t desc_base = (uint32_t)desc_addr;
	struct gdt_desc desc;
	desc.limit_low_word = limit & 0x0000ffff;
	desc.base_low_word = desc_base & 0x0000ffff;
	desc.base_mid_byte = ((desc_base & 0x00ff0000) >> 16);
	desc.attr_low_byte = (uint8_t)(attr_low);
	desc.limit_high_attr_high = (((limit & 0x000f0000) >> 16) + (uint8_t)(attr_high));
	desc.base_high_byte = desc_base >> 24;
	return desc;
}

static void __init page_create(void)/* reate page*/
{
	uint32_t phy_addr = 0;
	uint32_t cr0;
	/* set gdt */
	uint64_t gdt_operand = ((8 * 6 - 1) | ((uint64_t)(uint32_t)0xc0000900 << 16));   // 6个描述符大小

	for (int i = 0; i < 1024; i++) {
		pgd[i].pgd = 0;
	}

	pgd[0].pgd = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
	pgd[768].pgd = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
	// pgd[1023] = (uint32_t)pgd | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;

	for (int i = 0; i < 1024; i++) {
		pte[i].pte_low = 0;
	}
	//create pte
	for (int i = 0; i < 1024; i++) {
		pte[i].pte_low = phy_addr | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
		phy_addr += PAGE_SIZE;
	}

	// 更新全局 multiboot_t 指针
	glb_mboot_ptr = (multiboot_t *)((uint32_t)glb_mboot_ptr + PAGE_OFFSET);

	//set  gs
	*((struct gdt_desc*)0x918) = make_gdt_desc((uint32_t*)0xc00b8000, 0x00007, GDT_DATA_ATTR_LOW_DPL0, GDT_ATTR_HIGH);
	

	// 设置页表
	asm volatile ("mov %0, %%cr3" : : "r" (pgd));

	// 启用分页，将 cr0 寄存器的分页位置为 1 就好
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;

	asm volatile ("mov %0, %%cr0" : : "r" (cr0));
	asm volatile ("lgdt %0" : : "m" (gdt_operand));
	asm volatile ("call %0" : : "m" (flush));
	return;
}

// static void __init page_create(void)/* reate page*/
// {
//     for (int i = 0; i < 1024; i++) {
//         swapper_pg_dir[i].pgd = 0;
//         pte0[i].pte_low = 0;
//         pte1[i].pte_low = 0;
//     }

//     swapper_pg_dir[0].pgd = (uint32_t)pte0 | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//     swapper_pg_dir[1].pgd = (uint32_t)pte1 | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//     swapper_pg_dir[768].pgd = (uint32_t)pte0 | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//     swapper_pg_dir[769].pgd = (uint32_t)pte1 | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//     // pgd[1023] = (uint32_t)pgd | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
    
//     uint32_t phy_addr = 0;
//     // for (int i = 0; i < 1024; i++) {
//     //     pte[i].pte_low = 0;
//     // }
//     //create pmd
//     for (int i = 0; i < 1024; i++) {
//         pte0[i].pte_low = phy_addr | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//         // pte[i].pte_low = phy_addr | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//         phy_addr += PAGE_SIZE;
//     }
//     for(int i = 0; i < 1024; i++) {
//         pte1[i].pte_low = phy_addr | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
//         phy_addr += PAGE_SIZE;
//     }

//     // 更新全局 multiboot_t 指针
//     glb_mboot_ptr = (multiboot_t *)((uint32_t)glb_mboot_ptr + PAGE_OFFSET);

//     //set  gs
//     *((struct gdt_desc*)0x918) = make_gdt_desc((uint32_t*)0xc00b8000, 0x00007, GDT_DATA_ATTR_LOW_DPL0, GDT_ATTR_HIGH);

//     //set gdt
//     uint64_t gdt_operand = ((8 * 6 - 1) | ((uint64_t)(uint32_t)0xc0000900 << 16));   // 6个描述符大小

//     // 设置页表
//     asm volatile ("mov %0, %%cr3" : : "r" (swapper_pg_dir));
//     uint32_t cr0;
//     // 启用分页，将 cr0 寄存器的分页位置为 1 就好
//     asm volatile ("mov %%cr0, %0" : "=r" (cr0));
//     cr0 |= 0x80000000;

//     asm volatile ("mov %0, %%cr0" : : "r" (cr0));
//     asm volatile ("lgdt %0" : : "m" (gdt_operand));
//     asm volatile ("call %0" : : "m" (flush));
//     return;
// }


static void __init gdt_create(void)
{
	uint64_t gdt_operand = ((8 * 6 - 1) | ((uint64_t)(uint32_t)0x900 << 16));   // 7个描述符大小
	/* gdt段基址为0x900,把tss放到第4个位置,也就是0x900+0x20的位置 */
	/* gdt段基址为0x900,把tss放到第4个位置,也就是0x900+0x20的位置 900 908 910 918 920 928 930 */
	*((struct gdt_desc*)0x900) = make_gdt_desc((uint32_t*)0, 0, 0, 0);
	*((struct gdt_desc*)0x908) = make_gdt_desc((uint32_t*)0, 0xfffff, GDT_CODE_ATTR_LOW_DPL0, GDT_ATTR_HIGH);
	*((struct gdt_desc*)0x910) = make_gdt_desc((uint32_t*)0, 0xfffff, GDT_DATA_ATTR_LOW_DPL0, GDT_ATTR_HIGH);
	*((struct gdt_desc*)0x918) = make_gdt_desc((uint32_t*)0xb8000, 0x00007, GDT_DATA_ATTR_LOW_DPL0, GDT_ATTR_HIGH);
	/* 在gdt中添加dpl为3的数据段和代码段描述符 */
	*((struct gdt_desc*)0x920) = make_gdt_desc((uint32_t*)0, 0xfffff, GDT_CODE_ATTR_LOW_DPL3, GDT_ATTR_HIGH);
	*((struct gdt_desc*)0x928) = make_gdt_desc((uint32_t*)0, 0xfffff, GDT_DATA_ATTR_LOW_DPL3, GDT_ATTR_HIGH);

	/* gdt 16位的limit 32位的段基址 */
	asm volatile ("lgdt %0" : : "m" (gdt_operand));
	asm volatile ("call %0" : : "m" (flush));
	asm volatile ("mov %0, %%ax;mov %%ax, %%ds" : : "i" (SELECTOR_K_DATA));
	asm volatile ("mov %0, %%ax;mov %%ax, %%es" : : "i" (SELECTOR_K_DATA));
	asm volatile ("mov %0, %%ax;mov %%ax, %%ss" : : "i" (SELECTOR_K_DATA));
}

void __init kern_entry(void)
{
	uint32_t kern_stack_top;

	gdt_create();
	page_create();
	
	// 切换内核栈
	asm volatile ("mov %%esp, %0": "=r" (kern_stack_top));
	kern_stack_top += PAGE_OFFSET;
	asm volatile ("mov %0, %%esp": : "r" (kern_stack_top));
	
	// 调用内核初始化函数
	// while(1);
	start_kernel();
}
