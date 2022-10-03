#ifndef __CMAP_MEM_H__
#define __CMAP_MEM_H__

#include "cmap-mem-define.h"

typedef struct
{
  void * (*alloc)(int size);
  void (*free)(void * ptr);
} CMAP_MEM;

typedef struct
{
  int nb_chunk, nb_block, nb_block_free;
  int size_alloc, size_free;
} CMAP_MEM_STATE;

typedef struct
{
  CMAP_MEM * (*create)(int chunk_size);

  CMAP_MEM_STATE * (*state)();

  char (*is_this)(CMAP_MEM * mem);
} CMAP_MEM_PUBLIC;

extern const CMAP_MEM_PUBLIC cmap_mem_public;

#endif
