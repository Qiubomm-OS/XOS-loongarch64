#ifndef _ASM_IO_H
#define _ASM_IO_H

#include <asm/page.h>
#include <linux/types.h>
#include <linux/ioport.h>

static inline void * phys_to_virt(unsigned long address)
{
    return __va(address);
}
static inline unsigned long virt_to_phys(volatile void * address)
{
    return __pa(address);
}

#ifdef SLOW_IO_BY_JUMPING
#define __SLOW_DOWN_IO "\njmp 1f\n1:\tjmp 1f\n1:"
#else
#define __SLOW_DOWN_IO "\noutb %%al,$0x80"
#endif

#define virt_to_bus virt_to_phys
#define bus_to_virt phys_to_virt

#ifdef REALLY_SLOW_IO
#define __FULL_SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO __SLOW_DOWN_IO
#else
#define __FULL_SLOW_DOWN_IO __SLOW_DOWN_IO
#endif

extern  void outsb(unsigned short port, const void * addr, unsigned long count);
extern  void outsw(unsigned short port, const void * addr, unsigned long count);
extern  void outsl(unsigned short port, const void * addr, unsigned long count);

extern  void insb(unsigned short port, void * addr, unsigned long count);
extern  void insw(unsigned short port, void * addr, unsigned long count);
extern  void insl(unsigned short port, void * addr, unsigned long count);

extern  void outb(unsigned char value, unsigned short port);
extern  void outw(unsigned short value, unsigned short port);
extern  void outl(unsigned int value, unsigned short port);

extern  void outb_p(unsigned char value, unsigned short port);
extern  void outw_p(unsigned short value, unsigned short port);
extern  void outl_p(unsigned int value, unsigned short port);

extern  unsigned char inb(unsigned short port);
extern  unsigned short inw(unsigned short port);
extern  unsigned int inl(unsigned short port);

extern  unsigned char inb_p(unsigned short port);
extern  unsigned short inw_p(unsigned short port);
extern  unsigned int inl_p(unsigned short port);

#define IO_SPACE_LIMIT 0xffff
extern int request_resource(struct resource *root, struct resource *new);
/* PC/ISA/whatever - the normal PC address spaces: IO and memory */
extern struct resource ioport_resource;
extern struct resource iomem_resource;
#endif
