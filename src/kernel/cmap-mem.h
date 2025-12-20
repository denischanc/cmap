#ifndef __CMAP_MEM_H__
#define __CMAP_MEM_H__

#include "cmap-mem-type.h"
#include "cmap-mem-define.h"

typedef struct
{
  CMAP_MEM * (*instance)();

  CMAP_MEM_STATE * (*state)();

  char (*is_this)();

#ifdef CONSUMED_TIME
  void (*log_consumed_time)(char lvl);
#endif
} CMAP_MEM_PUBLIC;

extern const CMAP_MEM_PUBLIC cmap_mem_public;

#endif
