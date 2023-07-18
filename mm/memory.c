#include <linux/mm.h>

mem_map_t * mem_map;
unsigned long max_mapnr;   // 系统中的最大页面数
unsigned long num_physpages;   // 系统中的最大页面数
unsigned long num_mappedpages;   // 可能被内核映射的最大页面数
void * high_memory;
