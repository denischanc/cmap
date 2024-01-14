#ifndef __CMAP_MEM_TYPE_H__
#define __CMAP_MEM_TYPE_H__

#include <stdint.h>

typedef struct
{
  void * (*alloc)(int size);
  void (*free)(void * ptr);
} CMAP_MEM;

typedef struct
{
  int nb_chunk, nb_block, nb_block_free,
    size_alloc, size_free;
} CMAP_MEM_STATE;

#endif
