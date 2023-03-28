#ifndef _LINUX_NS16550A_H
#define _LINUX_NS16550A_H

#include <linux/types.h>

/* 初始化串口设备 */
void serial_ns16550a_init(uint64_t base_addr, uint32_t baud_rate);

/* 发送一个字符 */
void serial_ns16550a_putc(uint64_t base_addr, char c);

/* 接收一个字符 */
char serial_ns16550a_getc(uint64_t base_addr);

#endif /* _LINUX_NS16550A_H */
