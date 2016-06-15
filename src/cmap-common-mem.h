#ifndef __CMAP_COMMON_MEM_H__
#define __CMAP_COMMON_MEM_H__

#include "cmap-mem.h"

#define CMAP_ALLOC(struct, mem) \
  (struct *)mem -> alloc(sizeof(struct))

#define CMAP_ALLOC_PTR(ptr, struct, mem) \
  struct * ptr = CMAP_ALLOC(struct, mem)

#define CMAP_FREE(ptr, mem) \
  mem -> free((void *)ptr)

#endif
