#ifndef __CMAP_MEM_H__
#define __CMAP_MEM_H__

typedef struct
{
  void * (*alloc)(int size);
  void (*free)(void * ptr);
} CMAP_MEM;

typedef struct
{
  int nb_chunk_, nb_block_, nb_block_free_;
  int size_alloc_, size_free_;
} CMAP_MEM_STATE;

CMAP_MEM * cmap_mem_create(int chunk_size);

CMAP_MEM_STATE * cmap_mem_state();

char cmap_mem_is_this(CMAP_MEM * mem);

#endif
