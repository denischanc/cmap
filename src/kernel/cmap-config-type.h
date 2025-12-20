#ifndef __CMAP_CONFIG_TYPE_H__
#define __CMAP_CONFIG_TYPE_H__

#include <stdint.h>
#include "cmap-mem-type.h"
#include "cmap-log-type.h"

typedef struct
{
  CMAP_MEM * this;
  int chunk_size;
  char failure_on_alloc;
} CMAP_CONFIG_MEM;

typedef struct
{
  CMAP_LOG * this;
  char lvl;
  const char * path;
} CMAP_CONFIG_LOG;

typedef struct
{
  uint64_t check_zombie_time_us;
} CMAP_CONFIG_REFS;

typedef struct
{
  int list_chunk_size;
  int string_size_inc_min, string_size_inc;
} CMAP_CONFIG_CORE;

typedef struct
{
  int size;
} CMAP_CONFIG_POOL;

typedef struct
{
  CMAP_CONFIG_MEM mem;
  CMAP_CONFIG_LOG log;
  CMAP_CONFIG_REFS refs;
  CMAP_CONFIG_CORE core;
  CMAP_CONFIG_POOL pool;
} CMAP_CONFIG;

#endif
