#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-mem.h"

typedef struct
{
  CMAP_MEM * mem_;
} CMAP_KERNEL_CFG;

typedef struct
{
  CMAP_MEM * mem_;

  int (*main)(int argc, char * argv[]);

  void (*exit)(int ret);
  void (*fatal)();
} CMAP_KERNEL;

void cmap_kernel_create(CMAP_KERNEL_CFG * cfg);
CMAP_KERNEL * cmap_kernel();

#endif
