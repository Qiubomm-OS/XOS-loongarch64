#include <linux/mmzone.h>
#include <linux/bootmem.h>

int numnodes = 1;   /* Initialized for UMA platforms */

static bootmem_data_t contig_bootmem_data;
pg_data_t contig_page_data = { bdata: &contig_bootmem_data };
