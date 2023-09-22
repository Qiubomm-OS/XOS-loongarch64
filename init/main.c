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
#include <asm/bootinfo.h>
#include <asm/boot_param.h>
#include <linux/efi.h>

extern void __init __no_sanitize_address start_kernel(void);
extern int signature_cmp(uint64_t sig_num, char *type);
extern void parse_fwargs(int a0, char **args, efi_system_table_t *a2);

bool early_boot_irqs_disabled;

int signature_cmp(uint64_t sig_num, char *type)
{
	char *sig = (char *)&sig_num;
	while (*type != '\0') {
		if (*sig != *type) {
			return 0;//不相等
		}
		sig++;
		type++;
	}
	return 1;
}

void parse_fwargs(int a0, char **args, efi_system_table_t *a2)
{
    	printk("@@@@@: efi_system_table->%d\n", a2->nr_tables);
}

void __init __no_sanitize_address start_kernel(void)
{
	char str[] = "xos";
	int i;
	// int cpu = smp_processor_id();

	// serial_ns16550a_init(9600);
	printk("%s %s-%d.%d.%d\n", "hello", str, 0, 0, 1);

	printk("There is %d args for kernel:\n", fw_arg0);
	for (i = 0; i < fw_arg0; i++) {
		printk("cmd arg %d: %s\n", i, ((char **)fw_arg1)[i]);
	}
	printk("bpi = %x\n", fw_arg2);
	printk("efi system table at %x\n", ((struct bootparamsinterface *)fw_arg2)->systemtable);
    	printk("efi extend list at %x\n", ((struct bootparamsinterface *)fw_arg2)->extlist);
	parse_fwargs(fw_arg0, (char **)fw_arg1, (efi_system_table_t *)fw_arg2);

	// printk("@@@@@@: %d\n", BITS_PER_LONG);

	// local_irq_disable();
	// early_boot_irqs_disabled = true;

	/**
	 * 禁止中断，进行必要的设置后开启
	 */
	
	// printk("cpu = %d\n", cpu);

	while(1);
}
