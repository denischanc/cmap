#ifndef __CMAP_KERNEL_TYPE_H__
#define __CMAP_KERNEL_TYPE_H__

#include "cmap-mem-type.h"
#include "cmap-log-type.h"

typedef struct
{
  char failure_on_allocmem, log_lvl, delete_zombie;

  CMAP_MEM * mem;
  CMAP_LOG * log;
} CMAP_KERNEL_CFG;

#endif
