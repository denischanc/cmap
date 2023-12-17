#ifndef __CMAP_KERNEL_DEFINE_H__
#define __CMAP_KERNEL_DEFINE_H__

#include "cmap-mem.h"

#define CMAP_KERNEL_INSTANCE cmap_kernel_public.instance()
#define CMAP_KERNEL_MEM CMAP_KERNEL_INSTANCE -> mem()

#define CMAP_KERNEL_ALLOC(struct) CMAP_MEM_ALLOC(struct, CMAP_KERNEL_MEM)

#define CMAP_KERNEL_ALLOC_PTR(ptr, struct) \
  CMAP_MEM_ALLOC_PTR(ptr, struct, CMAP_KERNEL_MEM)

#define CMAP_KERNEL_FREE(ptr) CMAP_MEM_FREE(ptr, CMAP_KERNEL_MEM)

#endif
