#ifndef __CMAP_KERNEL_EXTERNAL_H__
#define __CMAP_KERNEL_EXTERNAL_H__

#include "cmap-mem-external.h"

typedef struct
{
  CMAP_MEM * mem;

  char failure_on_allocmem;
} CMAP_KERNEL_CFG;

#endif
