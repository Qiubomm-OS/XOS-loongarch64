#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/task.h>

union thread_union init_thread_union;

void start_kernel(void)
{
	// efi_puts("start_kernel.\n");
	local_irq_disable();
	while(1);
}
