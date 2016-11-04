#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-mem.h"
#include "cmap-map.h"
#include "cmap-prototype.h"
#include "cmap-warehouse.h"
#include "cmap-log.h"
#include "cmap-pool.h"

#define CMAP_KERNEL_S_UNKNOWN 0
#define CMAP_KERNEL_S_INIT 1
#define CMAP_KERNEL_S_ALIVE 2
#define CMAP_KERNEL_S_EXITING 3

typedef struct
{
  CMAP_MEM * mem_;

  char failure_on_allocmem_;
} CMAP_KERNEL_CFG;

typedef struct
{
  CMAP_PROTOTYPE prototype_;

  CMAP_MAP * global_env_;

  CMAP_WAREHOUSE * warehouse_;

  CMAP_POOL_LIST * pool_list_;
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

void cmap_kernel_init(CMAP_KERNEL_CFG * cfg);
CMAP_KERNEL * cmap_kernel();

#endif
