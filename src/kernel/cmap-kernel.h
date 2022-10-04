#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-mem.h"
#include "cmap-map.h"
#include "cmap-prototype.h"
#include "cmap-warehouse.h"
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
  CMAP_PROTOTYPE prototype_;

  CMAP_MAP * global_env_;

  CMAP_WAREHOUSE * warehouse_;

  CMAP_POOL_LIST * pool_list_;
  CMAP_POOL_STRING * pool_string_;
} CMAP_KERNEL_FW;

typedef struct
{
  CMAP_LOG log_;

  CMAP_KERNEL_CFG * cfg_;

  CMAP_MEM * mem_;

  CMAP_KERNEL_FW fw_;

  int (*main)(int argc, char * argv[]);

  void (*exit)(int ret);
  void (*fatal)();

  int (*state)();
} CMAP_KERNEL;

typedef struct
{
  void (*init)(CMAP_KERNEL_CFG * cfg);
  CMAP_KERNEL * (*this)();
} CMAP_KERNEL_PUBLIC;

extern const CMAP_KERNEL_PUBLIC cmap_kernel_public;

#endif
