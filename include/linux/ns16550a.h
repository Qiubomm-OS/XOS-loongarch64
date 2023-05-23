#ifndef _LINUX_NS16550A_H
#define _LINUX_NS16550A_H

#include <linux/types.h>

/* 初始化串口设备 */
void serial_ns16550a_init(uint64_t base_addr, uint32_t baud_rate);

/* 发送一个字符 */
void serial_ns16550a_putc(char c);

/* 发送一个字符串 */
void serial_ns16550a_puts(char *str);

/* 接收一个字符 */
char serial_ns16550a_getc(void);

#endif /* _LINUX_NS16550A_H */
