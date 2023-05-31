cmd_scripts/sorttable := gcc -Wp,-MMD,scripts/.sorttable.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11 -Wdeclaration-after-statement      -I./tools/include -I./tools/arch/x86/include -DUNWINDER_ORC_ENABLED   -o scripts/sorttable scripts/sorttable.c   -lpthread

source_scripts/sorttable := scripts/sorttable.c

deps_scripts/sorttable := \
  tools/include/tools/be_byteshift.h \
  tools/include/tools/le_byteshift.h \
  scripts/sorttable.h \
  tools/arch/x86/include/asm/orc_types.h \
  tools/include/linux/types.h \
    $(wildcard include/config/PHYS_ADDR_T_64BIT) \
  tools/include/linux/compiler.h \
  tools/include/linux/compiler_types.h \
  tools/include/linux/compiler-gcc.h \

scripts/sorttable: $(deps_scripts/sorttable)

$(deps_scripts/sorttable):
