#ifndef _LINUX_COMPILER_ATTRIBUTTES_H
#define _LINUX_COMPILER_ATTRIBUTTES_H

#define __noreturn                      __attribute__((__noreturn__))

#define __aligned(x)                    __attribute__((__aligned__(x)))
#define __packed                        __attribute__((__packed__))

#if __has_attribute(__fallthrough__)
# define fallthrough                    __attribute__((__fallthrough__))
#else
# define fallthrough                    do {} while (0)  /* fallthrough */
#endif

#endif /* _LINUX_COMPILER_ATTRIBUTTES_H */
