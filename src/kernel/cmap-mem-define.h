#ifndef __CMAP_MEM_DEFINE_H__
#define __CMAP_MEM_DEFINE_H__

#define CMAP_MEM_ALLOC(struct, mem) (struct *)mem -> alloc(sizeof(struct))

#define CMAP_MEM_ALLOC_PTR(ptr, struct, mem) \
  struct * ptr = CMAP_MEM_ALLOC(struct, mem)

#define CMAP_MEM_FREE(ptr, mem) mem -> free((void *)ptr)

#endif
