#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-mem.h"
#include "cmap-map.h"
#include "cmap-kernel-prototype.h"
#include "cmap-warehouse.h"

typedef struct
{
  CMAP_MEM * mem_;
} CMAP_KERNEL_CFG;

typedef struct
{
  CMAP_MEM * mem_;

  CMAP_KERNEL_PROTOTYPE prototype_;

  CMAP_MAP * global_env_;

  CMAP_WAREHOUSE * warehouse_;

  int (*main)(int argc, char * argv[]);

  void (*exit)(int ret);
  void (*fatal)();
} CMAP_KERNEL;

void cmap_kernel_create(CMAP_KERNEL_CFG * cfg);
CMAP_KERNEL * cmap_kernel();

#endif
