#ifndef __CMAP_KERNEL_DEFINE_H__
#define __CMAP_KERNEL_DEFINE_H__

#include "cmap-mem.h"

#define CMAP_KERNEL_ALLOC(struct) \
  CMAP_MEM_ALLOC(struct, cmap_kernel_public.this() -> mem_)

#define CMAP_KERNEL_ALLOC_PTR(ptr, struct) \
  CMAP_MEM_ALLOC_PTR(ptr, struct, cmap_kernel_public.this() -> mem_)

#define CMAP_KERNEL_FREE(ptr) \
  CMAP_MEM_FREE(ptr, cmap_kernel_public.this() -> mem_)

#endif
