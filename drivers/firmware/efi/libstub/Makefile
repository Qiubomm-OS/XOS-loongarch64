AS	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
LD	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-ld
LDFLAGS	=-m elf64loongarch --no-warn-rwx-segments -G0 -n -nostdlib
CC	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
CFLAGS	=-nostdinc -I../../../../arch/loongarch/include/ -I../../../../include/ -I../../../../arch/loongarch/include/uapi/ -I../../../../include/uapi\
	-Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing \
	-fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type \
	-Wno-format-security -funsigned-char -std=gnu11 -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding \
	-mno-check-zero-division -fno-asynchronous-unwind-tables -isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include \
	-fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member \
	-O2 -fno-allow-store-data-races -Wframe-larger-than=2048 -fstack-protector-strong -Wno-main -Wno-unused-but-set-variable \
	-Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer -ftrivial-auto-var-init=zero -fno-stack-clash-protection \
	-Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type -Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict \
	-Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -fno-strict-overflow -fno-stack-check -fconserve-stack \
	-Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING \
	-ffreestanding -fno-stack-protector -mdirect-extern-access -g -c

OBJS = loongarch-stub.o efi-stub-entry.o efi-stub-helper.o efi-stub.o loongarch.o mem.o printk.o screen_info.o systable.o vsprintf.o

libstub.o: $(OBJS)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJS)

loongarch-stub.o: loongarch-stub.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

efi-stub-entry.o: efi-stub-entry.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

efi-stub-helper.o: efi-stub-helper.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

efi-stub.o: efi-stub.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

loongarch.o: loongarch.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

mem.o: mem.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

printk.o: printk.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

screen_info.o: screen_info.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

systable.o: systable.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

vsprintf.o: vsprintf.c
	@echo 编译代码文件 $< ...
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: qemu
clean:
	rm -f $(OBJS) libstub.o