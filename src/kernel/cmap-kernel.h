#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include "cmap-kernel-type.h"
#include "cmap-kernel-define.h"

#define CMAP_KERNEL_S_UNKNOWN 0
#define CMAP_KERNEL_S_INIT 1
#define CMAP_KERNEL_S_ALIVE 2
#define CMAP_KERNEL_S_EXITING 3

typedef struct
{
  int (*main)();

  void (*exit)(int ret);
  void (*fatal)();

  CMAP_KERNEL_CFG * (*cfg)();
  CMAP_MEM * (*mem)();
  CMAP_LOG * (*log)();

  char (*state)();
} CMAP_KERNEL;

typedef struct
{
  CMAP_KERNEL_CFG * (*dft_cfg)();

  CMAP_KERNEL * (*bootstrap)(CMAP_KERNEL_CFG * cfg);
  CMAP_KERNEL * (*instance)();
} CMAP_KERNEL_PUBLIC;

extern const CMAP_KERNEL_PUBLIC cmap_kernel_public;

#endif
