#ifndef _LINUX_COMPILER_ATTRIBUTTES_H
#define _LINUX_COMPILER_ATTRIBUTTES_H

#define __noreturn                      __attribute__((__noreturn__))

#define __aligned(x)                    __attribute__((__aligned__(x)))
#define __packed                        __attribute__((__packed__))

#endif /* _LINUX_COMPILER_ATTRIBUTTES_H */
