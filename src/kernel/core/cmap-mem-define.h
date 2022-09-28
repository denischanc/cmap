#ifndef __CMAP_MEM_DEFINE_H__
#define __CMAP_MEM_DEFINE_H__

#define CMAP_ALLOC(struct, mem) \
  (struct *)mem -> alloc(sizeof(struct))

#define CMAP_ALLOC_PTR(ptr, struct, mem) \
  struct * ptr = CMAP_ALLOC(struct, mem)

#define CMAP_FREE(ptr, mem) \
  mem -> free((void *)ptr)

#endif
