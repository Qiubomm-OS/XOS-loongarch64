#ifndef _ASM_IRQFLAGS_H
#define _ASM_IRQFLAGS_H

#include <asm/system.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <asm/gdt.h>

#ifndef __ASSEMBLY__

extern inline unsigned long native_save_fl(void);
extern __always_inline unsigned long native_save_fl(void)
{
	unsigned long flags;

	save_flags(flags);

	return flags;
}

static __always_inline void native_irq_disable(void)
{
	cli();
}

static __always_inline void native_irq_enable(void)
{
	sti();
}

static __always_inline void native_safe_halt(void)
{
	asm volatile("sti; hlt": : :"memory");
}

static __always_inline void native_halt(void)
{
	asm volatile("hlt": : :"memory");
}

#endif /* !__ASSEMBLY__ */

#ifndef __ASSEMBLY__

static __always_inline unsigned long arch_local_save_flags(void)
{
	return native_save_fl();
}

static __always_inline void arch_local_irq_disable(void)
{
	native_irq_disable();
}

static __always_inline void arch_local_irq_enable(void)
{
	native_irq_enable();
}

/*
 * 在idle循环中使用; sti指令需要一个时钟周期才能完成:
 */
static __always_inline void arch_safe_halt(void)
{
	native_safe_halt();
}

/*
 * 当中断已启用使用，或者为关闭处理器使用:
 */
static __always_inline void halt(void)
{
	native_halt();
}

/*
 * 用于自旋锁，etc:
 */
static __always_inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}

#endif /* !__ASSEMBLY__ */

#ifndef __ASSEMBLY__

static __always_inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & EFLAGS_IF);
}

static __always_inline int arch_irqs_disabled(void)
{
	unsigned long flags = arch_local_save_flags();

	return arch_irqs_disabled_flags(flags);
}

static __always_inline void arch_local_irq_restore(unsigned long flags)
{
	if (!arch_irqs_disabled_flags(flags))
		arch_local_irq_enable();
}

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_IRQFLAGS_H */
