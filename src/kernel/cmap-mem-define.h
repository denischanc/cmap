#ifndef __CMAP_MEM_DEFINE_H__
#define __CMAP_MEM_DEFINE_H__

#define CMAP_MEM_ALLOC(struct, mem) (struct *)mem -> alloc(sizeof(struct))

#define CMAP_MEM_ALLOC_PTR(ptr, struct, mem) \
  struct * ptr = CMAP_MEM_ALLOC(struct, mem)

#define CMAP_MEM_FREE(ptr, mem) mem -> free((void *)ptr)

#define CMAP_MEM_INSTANCE cmap_mem_public.instance()
#define CMAP_MEM_VAR CMAP_MEM * mem = CMAP_MEM_INSTANCE

#define CMAP_MEM_INSTANCE_ALLOC(struct) \
  CMAP_MEM_ALLOC(struct, CMAP_MEM_INSTANCE)

#define CMAP_MEM_INSTANCE_ALLOC_PTR(ptr, struct) \
  CMAP_MEM_ALLOC_PTR(ptr, struct, CMAP_MEM_INSTANCE)

#define CMAP_MEM_INSTANCE_FREE(ptr) CMAP_MEM_FREE(ptr, CMAP_MEM_INSTANCE)

#define CMAP_MEM_CHUNK_SIZE_DFT 1 << 20

#endif
