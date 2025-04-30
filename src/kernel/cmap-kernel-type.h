#ifndef __CMAP_KERNEL_TYPE_H__
#define __CMAP_KERNEL_TYPE_H__

#include <stdint.h>
#include "cmap-mem-type.h"
#include "cmap-log-type.h"

typedef struct
{
  CMAP_MEM * this;
  int chunk_size;
  char failure_on_alloc;
} CMAP_KERNEL_MEM_CFG;

typedef struct
{
  CMAP_LOG * this;
  char lvl;
} CMAP_KERNEL_LOG_CFG;

typedef struct
{
  uint64_t check_zombie_time_us;
} CMAP_KERNEL_REFS_CFG;

typedef struct
{
  int list_chunk_size;
  int string_size_inc_min, string_size_inc;
} CMAP_KERNEL_CORE_CFG;

typedef struct
{
  int size;
} CMAP_KERNEL_POOL_CFG;

typedef struct
{
  CMAP_KERNEL_MEM_CFG mem;
  CMAP_KERNEL_LOG_CFG log;
  CMAP_KERNEL_REFS_CFG refs;
  CMAP_KERNEL_CORE_CFG core;
  CMAP_KERNEL_POOL_CFG pool;
} CMAP_KERNEL_CFG;

#endif
