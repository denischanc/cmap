#ifndef __COMMON_H__
#define __COMMON_H__

#define CMAP_T (1 == 1)
#define CMAP_F (1 == 0)

#define CMAP_ALLOC(size) \
  cmap_kernel() -> mem_ -> alloc(size)

#define CMAP_ALLOC_STRUCT(struct) \
  (struct *)cmap_kernel() -> mem_ -> alloc(sizeof(struct))

#define CMAP_FREE(ptr) \
  cmap_kernel() -> mem_ -> free((void *)ptr)

#define CMAP_CALL(e, fn) e -> fn(e)
#define CMAP_CALL_ARGS(e, fn, args) e -> fn(e, args)

#endif
