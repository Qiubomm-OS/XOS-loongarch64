#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/task.h>
#include <linux/init.h>
#include <linux/ns16550a.h>
#include <linux/types.h>
#include <linux/stdio.h>
#include <linux/smp.h>
#include <asm-generic/bitsperlong.h>

extern void __no_sanitize_address start_kernel(void);
extern void setup_arch(void);

bool early_boot_irqs_disabled;

void __init __no_sanitize_address start_kernel(void)
{
	char str[] = "xos";
	// int cpu = smp_processor_id();

#ifdef CONFIG_LOONGARCH	
	serial_ns16550a_init(9600);
#endif
	printk("%s %s-%d.%d.%d\n", "hello", str, 0, 0, 1);
	setup_arch();

	// local_irq_disable();
	// early_boot_irqs_disabled = true;

	/**
	 * 禁止中断，进行必要的设置后开启
	 */
	
	// printk("cpu = %d\n", cpu);

	while(1);
}
