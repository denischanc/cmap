#ifndef ___CMAP_KERNEL_H___
#define ___CMAP_KERNEL_H___

#include <cmap/mem.h>

typedef struct
{
  CMAP_MEM * mem;

  char failure_on_allocmem;
} CMAP_KERNEL_CFG;

#endif
