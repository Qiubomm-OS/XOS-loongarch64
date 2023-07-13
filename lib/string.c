#include <linux/string.h>
#include <linux/printk.h>

void memset(void* dst_, uint8_t value, uint32_t size) {
	uint8_t* dst = (uint8_t*)dst_;
	while (size-- > 0)
		*dst++ = value;
}

void memcpy(void* dst_, const void* src_, uint32_t size)
{
	uint8_t* dst = dst_;
	const uint8_t* src = src_;
	while (size-- > 0)
	*dst++ = *src++;
}

int memcmp(const void* a_, const void* b_, uint32_t size)
{
	const char* a = a_;
	const char* b = b_;
	while (size-- > 0) {
		if(*a != *b) {
			return *a > *b ? 1 : -1; 
		}
		a++;
		b++;
	}
	return 0;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
	/* nothing */;
	return tmp;
}

uint32_t strlen(const char* str)
{
	const char* p = str;
	while(*p++);
	return (p - str - 1);
}

uint32_t strnlen(const char* str, uint32_t max)
{
	uint32_t len = strlen(str);
	return len >= max ? max : len;
}
