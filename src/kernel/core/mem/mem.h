#ifndef __MEM_H__
#define __MEM_H__

#define CMAP_T (1 == 1)
#define CMAP_F (1 == 0)

typedef struct CMAP_MEM_s CMAP_MEM;

struct CMAP_MEM_s
{
  void * internal_;

  void * (*alloc)(CMAP_MEM * mem, int size);
  void (*free)(CMAP_MEM * mem, void * ptr);
};

typedef struct
{
  CMAP_MEM * (*create)(int chunk_size);
} CMAP_MEM_FACTORY;

CMAP_MEM_FACTORY * cmap_mem_factory();

#endif
