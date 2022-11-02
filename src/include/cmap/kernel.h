#ifndef ___CMAP_KERNEL_H___
#define ___CMAP_KERNEL_H___

#include <cmap/mem.h>
#include <cmap/log.h>

typedef struct
{
  char failure_on_allocmem;

  CMAP_MEM * mem;
  CMAP_LOG * log;
} CMAP_KERNEL_CFG;

#endif
