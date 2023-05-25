#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/task.h>
#include <linux/init.h>
#include <linux/ns16550a.h>
#include <linux/types.h>
#include <asm/stdio.h>

bool early_boot_irqs_disabled;

union thread_union init_thread_union;

void __init __no_sanitize_address start_kernel(void)
{
	char str[] = "XOS-loongarch64";
	local_irq_disable();
	serial_ns16550a_init(0x1fe001e0, 9600);
	printk("%s %s-%d.%d.%d\n", "hello", str, 0, 0, 1);

	early_boot_irqs_disabled = true;

	while(1);
}
