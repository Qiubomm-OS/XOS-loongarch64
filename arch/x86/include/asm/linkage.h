#ifndef _ASM_LINKAGE_H
#define _ASM_LINKAGE_H

#define asmlinkage __attribute__((regparm(0)))
#define SYMBOL_NAME_STR(X) #X
#define SYMBOL_NAME(X) X
#define __ALIGN .align 4,0x90
#define __ALIGN_STR ".align 4,0x90"
#define ALIGN __ALIGN
#define ENTRY(name) \
  .globl name; \
  ALIGN; \
  name:

#endif /* _ASM_LINKAGE_H */
