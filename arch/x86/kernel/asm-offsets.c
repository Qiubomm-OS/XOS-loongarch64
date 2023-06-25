#include <linux/compiler.h>
#include <linux/kbuild.h>

#include <asm/page.h>

int main(void)
{
        BLANK();
        DEFINE(PAGE_SZ, PAGE_SIZE);
        BLANK();

        return 0;
}