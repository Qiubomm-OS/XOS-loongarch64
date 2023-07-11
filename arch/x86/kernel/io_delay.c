#include <asm/io.h>

#define IO_DELAY_TYPE_0X80	0
#define IO_DELAY_TYPE_0XED	1
#define IO_DELAY_TYPE_UDELAY	2
#define IO_DELAY_TYPE_NONE	3

int io_delay_type = CONFIG_DEFAULT_IO_DELAY_TYPE;

void native_io_delay(void)
{
	switch (io_delay_type) {
	case IO_DELAY_TYPE_0X80:
		asm volatile ("outb %al, $0x80");
		break;
	case IO_DELAY_TYPE_0XED:
		asm volatile ("outb %al, $0xed");
		break;
	case IO_DELAY_TYPE_UDELAY:
		// udelay(2);
	case IO_DELAY_TYPE_NONE:
		break;
	}
}
