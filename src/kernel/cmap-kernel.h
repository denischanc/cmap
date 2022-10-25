#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-mem.h"
#include "cmap-map.h"
#include "cmap-prototype.h"
#include "cmap-aislestore.h"
#include "cmap-log.h"
#include "cmap-pool.h"
#include "cmap-kernel-define.h"
#include "cmap-kernel-external.h"

#define CMAP_KERNEL_S_UNKNOWN 0
#define CMAP_KERNEL_S_INIT 1
#define CMAP_KERNEL_S_ALIVE 2
#define CMAP_KERNEL_S_EXITING 3

typedef struct
{
  CMAP_MEM * mem;
  CMAP_LOG * log;

  CMAP_PROTOTYPE prototype;
  CMAP_MAP * global_env;
  CMAP_AISLESTORE * aislestore;

  CMAP_POOL_LIST * pool_list;
  CMAP_POOL_STRING * pool_string;

  int (*main)(int argc, char * argv[]);

  void (*exit)(int ret);
  void (*fatal)();

  int (*state)();
} CMAP_KERNEL;

typedef struct
{
  CMAP_KERNEL * (*init)(CMAP_KERNEL_CFG * cfg);
  CMAP_KERNEL * (*instance)();
} CMAP_KERNEL_PUBLIC;

extern const CMAP_KERNEL_PUBLIC cmap_kernel_public;

#endif
