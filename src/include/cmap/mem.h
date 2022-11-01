#ifndef ___CMAP_MEM_H___
#define ___CMAP_MEM_H___

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

#endif
