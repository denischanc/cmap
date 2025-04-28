#ifndef __CMAP_KERNEL_TYPE_H__
#define __CMAP_KERNEL_TYPE_H__

#include <stdint.h>
#include "cmap-mem-type.h"
#include "cmap-log-type.h"

typedef struct
{
  char failure_on_allocmem, log_lvl;

  uint64_t check_zombie_time_us;

  CMAP_MEM * mem;
  CMAP_LOG * log;
} CMAP_KERNEL_CFG;

#endif
