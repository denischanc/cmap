#ifndef __CMAP_COMMON_H__
#define __CMAP_COMMON_H__

#include "cmap-mem.h"
#include "cmap-kernel.h"
#include "cmap-map.h"

#define CMAP_T (1 == 1)
#define CMAP_F (1 == 0)

#define CMAP_ALLOC(struct, mem) \
  (struct *)mem -> alloc(sizeof(struct))
#define CMAP_ALLOC_PTR(ptr, struct, mem) \
  struct * ptr = CMAP_ALLOC(struct, mem)

#define CMAP_KERNEL_ALLOC(struct) \
  CMAP_ALLOC(struct, cmap_kernel() -> mem_)
#define CMAP_KERNEL_ALLOC_PTR(ptr, struct) \
  CMAP_ALLOC_PTR(ptr, struct, cmap_kernel() -> mem_)

#define CMAP_FREE(ptr, mem) \
  mem -> free((void *)ptr)
#define CMAP_KERNEL_FREE(ptr) \
  CMAP_FREE(ptr, cmap_kernel() -> mem_)

#define CMAP_CALL(e, fn) (e) -> fn(e)
#define CMAP_CALL_ARGS(e, fn, args...) (e) -> fn(e, args)

#define CMAP_NEW_MAP(prototype, aisle) \
  (CMAP_MAP *)CMAP_CALL_ARGS( \
    (CMAP_MAP *)prototype, new, sizeof(CMAP_MAP), aisle)

#define CMAP_PUSH(list, val) \
  CMAP_CALL_ARGS(list, push, (CMAP_MAP *)val)
#define CMAP_SHIFT(list, val) \
  CMAP_CALL_ARGS(list, shift, (CMAP_MAP *)val)

#define CMAP_NATURE(map) \
  CMAP_CALL(map, nature)

#define CMAP_DELETE(map) \
  CMAP_CALL((CMAP_MAP *)map, delete)

#endif
