#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/task.h>
#include <linux/init.h>

union thread_union init_thread_union;

void __init __no_sanitize_address start_kernel(void)
{
	local_irq_disable();
	while(1);
}
