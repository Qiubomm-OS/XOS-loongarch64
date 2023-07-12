#include <linux/init.h>
#include <linux/stdio.h>
#include <asm/multiboot.h>

static void show_memory_map(void)
{
	uint32_t mmap_addr = ((multiboot_t*)glb_mboot_ptr)->mmap_addr;
	uint32_t mmap_length = ((multiboot_t*)glb_mboot_ptr)->mmap_length;
	mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;

	printk("Memory map:\n");

	for (mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_length; mmap++) {
		printk("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
		(uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
		(uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
		(uint32_t)mmap->type);
	}
}

extern void __init setup_arch(void);
void __init setup_arch(void)
{
	// unsigned long max_low_pfn;
	show_memory_map();
}
