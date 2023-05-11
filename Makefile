AS	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
LD	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-ld
LDFLAGS	=-m elf64loongarch --no-warn-rwx-segments -T ./arch/loongarch/kernel/vmlinux.lds -Map ./build/kernel.map -G0 -n -nostdlib
CC	=/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
OBJCOPY = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-objcopy
CFLAGS	=-nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi -I./include/uapi\
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

all:	Image

Image: arch/loongarch/kernel/kernel.o drivers/firmware/efi/libstub/libstub.o drivers/serial/serial.o lib/lib.o init/init.o
	mkdir -p build/
	@$(LD) $(LDFLAGS) arch/loongarch/kernel/kernel.o drivers/firmware/efi/libstub/libstub.o drivers/serial/serial.o lib/lib.o init/init.o -o kernel.o
	$(OBJCOPY) -O binary --remove-section=.comment --remove-section=.note \
		--remove-section=.options --remove-section=.note.gnu.build-id \
		-S kernel.o kernel.efi

arch/loongarch/kernel/kernel.o:
	(cd arch/loongarch/kernel; make)

drivers/firmware/efi/libstub/libstub.o:
	(cd drivers/firmware/efi/libstub; make)

drivers/serial/serial.o:
	(cd drivers/serial; make)

lib/lib.o:
	(cd lib; make)

init/init.o:
	(cd init; make)
