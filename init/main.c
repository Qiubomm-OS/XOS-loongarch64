#include <linux/irqflags.h>

void start_kernel(void)
{
	local_irq_disable();
	while(1);
}
