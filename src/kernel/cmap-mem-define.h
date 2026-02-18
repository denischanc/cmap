#ifndef __CMAP_MEM_DEFINE_H__
#define __CMAP_MEM_DEFINE_H__

#define CMAP_MEM_ALLOC(struct) cmap_mem_alloc(sizeof(struct))

#define CMAP_MEM_ALLOC_PTR(ptr, struct) \
  struct * ptr = CMAP_MEM_ALLOC(struct)

#define CMAP_MEM_CHUNK_SIZE_DFT 1 << 20

#endif
