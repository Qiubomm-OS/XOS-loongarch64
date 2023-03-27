#include <linux/types.h>

/* 串口寄存器偏移地址 */
#define UART_RX      0       // 接收数据寄存器
#define UART_TX      0       // 发送数据寄存器
#define UART_IER     1       // 中断使能寄存器
#define UART_IIR     2       // 中断状态寄存器
#define UART_LCR     3       // 线路控制寄存器
#define UART_MCR     4       // 调制解调器控制寄存器
#define UART_LSR     5       // 线路状态寄存器
#define UART_MSR     6       // 调制解调器状态寄存器
#define UART_SR      7       // Scratch Register

/* 线路控制寄存器 (LCR) */
#define UART_LCR_DLAB  0x80  // Divisor Latch Bit

/* 线路状态寄存器 (LSR) */
#define UART_LSR_DR   0x01   // Data Ready
#define UART_LSR_THRE 0x20   // Transmit Holding Register Empty

/* 波特率计算公式：Baud = Clock / (16 * Divisor) */
uint16_t divisor = 0;       // 用于存储波特率分频系数

/* 初始化串口设备 */
// void serial_init(uint32_t base_addr, uint32_t baud_rate)
// {
// 	uint16_t divisor_value = 0;
//    	uint8_t lcr_value = 0;

// 	/* 计算波特率分频系数 */
// 	divisor_value = 115200 / baud_rate;
// 	divisor = divisor_value;

// 	/* 禁用中断 */
// 	outb(0x00, base_addr + UART_IER);

// 	/* 设置波特率 */
// 	lcr_value = inb(base_addr + UART_LCR);
// 	outb(lcr_value | UART_LCR_DLAB, base_addr + UART_LCR);
// 	outb(divisor_value & 0xff, base_addr + UART_DIV_LATCH_LOW);
// 	outb((divisor_value >> 8) & 0xff, base_addr + UART_DIV_LATCH_HIGH);
// 	outb(lcr_value, base_addr + UART_LCR);

// 	/* 设置数据位、停止位、校验位等参数 */
// 	outb(0x03, base_addr + UART_LCR);   // 8 数据位，1 停止位，无校验
// 	outb(0xc7, base_addr + UART_MCR);   // 打开 DTR、RTS 和 OUT2 信号

// 	/* 清空串口缓冲区 */
// 	while (inb(base_addr + UART_LSR) & UART_LSR_DR)
// 		inb(base_addr + UART_RX);
// }
