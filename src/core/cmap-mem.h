#ifndef __CMAP_MEM_H__
#define __CMAP_MEM_H__

#include "cmap-mem-define.h"
#include "cmap-mem-external.h"

typedef struct
{
  CMAP_MEM * (*create)(int chunk_size);

  CMAP_MEM_STATE * (*state)();

  char (*is_this)(CMAP_MEM * mem);
} CMAP_MEM_PUBLIC;

extern const CMAP_MEM_PUBLIC cmap_mem_public;

#endif
