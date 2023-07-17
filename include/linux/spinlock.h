#ifndef __LINUX_SPINLOCK_H
#define __LINUX_SPINLOCK_H

#include <asm/system.h>
#include <asm/bitops.h>
#include <asm/atomic.h>

typedef struct {
    int lock;
} spinlock_t;

typedef struct {
    spinlock_t lock;
} spinlock_cacheline_t;

#define local_irq_disable()	__cli()
#define local_irq_enable()  __sti()
#define spin_lock_init(lock)	do { } while(0)
#define spin_lock(lock)		(void)(lock) /* Not "unused variable". */
#define spin_is_locked(lock)	(0)
#define spin_trylock(lock)	({1; })
#define spin_unlock_wait(lock)	do { } while(0)
#define spin_unlock(lock)	do { } while(0)

#define spin_lock_irqsave(lock, flags)		do { local_irq_save(flags);       spin_lock(lock); } while (0)
#define spin_lock_irq(lock)			do { local_irq_disable();         spin_lock(lock); } while (0)
#define spin_lock_bh(lock)			do { local_bh_disable();          spin_lock(lock); } while (0)

#define read_lock_irqsave(lock, flags)		do { local_irq_save(flags);       read_lock(lock); } while (0)
#define read_lock_irq(lock)			do { local_irq_disable();         read_lock(lock); } while (0)
#define read_lock_bh(lock)			do { local_bh_disable();          read_lock(lock); } while (0)

#define write_lock_irqsave(lock, flags)		do { local_irq_save(flags);      write_lock(lock); } while (0)
#define write_lock_irq(lock)			do { local_irq_disable();        write_lock(lock); } while (0)
#define write_lock_bh(lock)			do { local_bh_disable();         write_lock(lock); } while (0)

#define spin_unlock_irqrestore(lock, flags)	do { spin_unlock(lock);  local_irq_restore(flags); } while (0)
#define spin_unlock_irq(lock)			do { spin_unlock(lock);  local_irq_enable();       } while (0)
#define spin_unlock_bh(lock)			do { spin_unlock(lock);  local_bh_enable();        } while (0)

#define read_unlock_irqrestore(lock, flags)	do { read_unlock(lock);  local_irq_restore(flags); } while (0)
#define read_unlock_irq(lock)			do { read_unlock(lock);  local_irq_enable();       } while (0)
#define read_unlock_bh(lock)			do { read_unlock(lock);  local_bh_enable();        } while (0)

#define write_unlock_irqrestore(lock, flags)	do { write_unlock(lock); local_irq_restore(flags); } while (0)
#define write_unlock_irq(lock)			do { write_unlock(lock); local_irq_enable();       } while (0)
#define write_unlock_bh(lock)			do { write_unlock(lock); local_bh_enable();        } while (0)

#define atomic_dec_and_lock(atomic,lock) atomic_dec_and_test(atomic)
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 0 }



#define rwlock_init(lock)	do { } while(0)
#define read_lock(lock)		(void)(lock) /* Not "unused variable". */
#define read_unlock(lock)	do { } while(0)
#define write_lock(lock)	(void)(lock) /* Not "unused variable". */
#define write_unlock(lock)	do { } while(0)

typedef struct { } rwlock_t;
#define RW_LOCK_UNLOCKED (rwlock_t) { }

#endif /* __LINUX_SPINLOCK_H */
