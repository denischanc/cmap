#ifndef __CMAP_KERNEL_H__
#define __CMAP_KERNEL_H__

#include <cmap/kernel.h>
#include "cmap-kernel-define.h"
#include "cmap-map.h"
#include "cmap-mem.h"
#include "cmap-log.h"
#include <uv.h>

#define CMAP_KERNEL_S_UNKNOWN 0
#define CMAP_KERNEL_S_INIT 1
#define CMAP_KERNEL_S_ALIVE 2
#define CMAP_KERNEL_S_EXITING 3

typedef struct
{
  int (*main)();

  void (*exit)(int ret);
  void (*fatal)();

  char (*state)();
} CMAP_KERNEL;

typedef struct
{
  CMAP_KERNEL * (*bootstrap)(CMAP_KERNEL_CFG * cfg);
  CMAP_KERNEL * (*instance)();

  CMAP_MEM * (*mem)();
  CMAP_LOG * (*log)();

  uv_loop_t * (*uv_loop)();
} CMAP_KERNEL_PUBLIC;

extern const CMAP_KERNEL_PUBLIC cmap_kernel_public;

#endif
