#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/task.h>
#include <linux/init.h>
#include <linux/ns16550a.h>

union thread_union init_thread_union;

void __init __no_sanitize_address start_kernel(void)
{
	local_irq_disable();
	serial_ns16550a_init(0x1fe001e0, 9600);
	serial_ns16550a_putc(0x1fe001e0, 'x');
	while(1);
}
