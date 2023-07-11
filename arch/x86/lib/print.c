#include <asm/stdio.h>
#include <linux/io.h>

/*
颜色生成方法
MAKE_COLOR(BLUE, RED)
MAKE_COLOR(BLACK, RED) | BRIGHT
MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
*/
#define TEXT_BLACK		0x0     /* 0000 */
#define TEXT_WHITE		0x7     /* 0111 */
#define TEXT_RED		0x4     /* 0100 */
#define TEXT_GREEN		0x2     /* 0010 */
#define TEXT_BLUE		0x1     /* 0001 */
#define TEXT_FLASH		0x80    /* 1000 0000 */
#define TEXT_BRIGHT		0x08    /* 0000 1000 */
#define	MAKE_COLOR(x,y)		(((x) << 4) | (y))   /* MAKE_COLOR(Background,Foreground) */

#define DISPLAY_VRAM		0xc00b8000

#define	CRTC_ADDR_REG		0x3D4	/* CRT Controller Registers - Addr Register */
#define	CRTC_DATA_REG		0x3D5	/* CRT Controller Registers - Data Register */
#define	START_ADDR_H		0xC	/* reg index of video mem start addr (MSB) */
#define	START_ADDR_L		0xD	/* reg index of video mem start addr (LSB) */
#define	CURSOR_H		0xE	/* reg index of cursor position (MSB) */
#define	CURSOR_L		0xF	/* reg index of cursor position (LSB) */
#define	V_MEM_BASE		DISPLAY_VRAM	/* base of color video memory */

#define SCREEN_UP		-1
#define SCREEN_DOWN		1

#define SCREEN_WIDTH		80
#define SCREEN_HEIGHT		25

#define COLOR_DEFAULT		(MAKE_COLOR(TEXT_BLACK, TEXT_WHITE))

int cursor_x = 0, cursor_y = 0;

static void set_cursor(unsigned short cursor)
{
	//设置光标位置 0-2000
	outb_p(CURSOR_H, CRTC_ADDR_REG);			//光标高位
	outb_p((cursor >> 8) & 0xFF, CRTC_DATA_REG);
	outb_p(CURSOR_L,CRTC_ADDR_REG);			//光标低位
	outb_p(cursor & 0xFF, CRTC_DATA_REG);
}

/**
 * flush - 刷新光标和起始位置
 * @console: 控制台
 */
static void flush(void)
{
	/* 计算光标位置，并设置 */
	set_cursor(cursor_y * SCREEN_WIDTH + cursor_x);
}

/**
 * scroll_sceen - 滚屏
 * @console: 控制台
 * @direction: 滚动方向
 *             - SCREEN_UP: 向上滚动
 *             - SCREEN_DOWN: 向下滚动
 * 
 */
static void scroll_sceen(int direction)
{
	unsigned char *vram = (unsigned char *)(V_MEM_BASE);
	int i;
	
	if (direction == SCREEN_UP) {
		/* 起始地址 */
		for (i = SCREEN_WIDTH * 2 * 24; i > SCREEN_WIDTH * 2; i -= 2) {
			vram[i] = vram[i - SCREEN_WIDTH * 2];
			vram[i + 1] = vram[i + 1 - SCREEN_WIDTH * 2];
		}
		for (i = 0; i < SCREEN_WIDTH * 2; i += 2) {
			vram[i] = '\0';
			vram[i + 1] = COLOR_DEFAULT;
		}
	} else if (direction == SCREEN_DOWN){
		/* 起始地址 */
		for (i = 0; i < SCREEN_WIDTH * 2 * 24; i += 2) {
			vram[i] = vram[i + SCREEN_WIDTH * 2];
			vram[i + 1] = vram[i + 1 + SCREEN_WIDTH * 2];
		}
		for (i = SCREEN_WIDTH * 2 * 24; i < SCREEN_WIDTH * 2 * 25; i += 2) {
			vram[i] = '\0';
			vram[i + 1] = COLOR_DEFAULT;
		}
		cursor_y--;
	}
	flush();
}

/**
 * put_char - 控制台上输出一个字符
 * @console: 控制台
 * @ch: 字符
 */
static void put_char(char ch)
{
	char *vram = (char *)(V_MEM_BASE + (cursor_y * SCREEN_WIDTH + cursor_x) * 2);
	switch (ch) {
        case '\r':
		break;
        case '\n':
		// 如果是回车，那还是要把回车写进去
		*vram++ = '\0';
		*vram = COLOR_DEFAULT;
		cursor_x = 0;
		cursor_y++;
		break;
        case '\b':
		if (cursor_x >= 0 && cursor_y >= 0) {
			cursor_x--;
			/* 调整为上一行尾 */
			if (cursor_x < 0) {
				cursor_x = SCREEN_WIDTH - 1;
				cursor_y--;
				/* 对y进行修复 */
				if (cursor_y < 0) {
					cursor_y = 0;
				}
			}
			*(vram-2) = '\0';
			*(vram-1) = COLOR_DEFAULT;
		}
		break;
	default:
		*vram++ = ch;
		*vram = COLOR_DEFAULT;
		
		cursor_x++;
		if (cursor_x > SCREEN_WIDTH - 1) {
			cursor_x = 0;
			cursor_y++;
		}
		break;
	}
	
	/* 滚屏 */
	while (cursor_y > SCREEN_HEIGHT - 1) {
		scroll_sceen(SCREEN_DOWN);
	}
	
	flush();
}

void print_str(char *str)
{
	while (*str) {
		put_char(*str);
		str++;
	}
}
