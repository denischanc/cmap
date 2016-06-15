#ifndef __CMAP_COMMON_KERNEL_H__
#define __CMAP_COMMON_KERNEL_H__

#include "cmap-kernel.h"

#define CMAP_KERNEL_ALLOC(struct) \
  CMAP_ALLOC(struct, cmap_kernel() -> mem_)

#define CMAP_KERNEL_ALLOC_PTR(ptr, struct) \
  CMAP_ALLOC_PTR(ptr, struct, cmap_kernel() -> mem_)

#define CMAP_KERNEL_FREE(ptr) \
  CMAP_FREE(ptr, cmap_kernel() -> mem_)

#endif
