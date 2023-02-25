#include <linux/string.h>
#include <linux/printk.h>

void memset(void* dst_, uint8_t value, uint32_t size) {
   if(dst_ == NULL) {
       efi_puts("BUG!!!");
       while(1);
   }
   uint8_t* dst = (uint8_t*)dst_;
   while (size-- > 0)
      *dst++ = value;
}

void memcpy(void* dst_, const void* src_, uint32_t size) {
   if(dst_ == NULL || src_ == NULL) {
       efi_puts("BUG!!!");
       while(1);
   }
   uint8_t* dst = dst_;
   const uint8_t* src = src_;
   while (size-- > 0)
      *dst++ = *src++;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
