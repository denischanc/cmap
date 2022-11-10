#ifndef __CMAP_SCHEDULER_H__
#define __CMAP_SCHEDULER_H__

#include <cmap/core.h>
#include <uv.h>

typedef struct
{
  CMAP_MAP * (*create)();

  void (*schedule)(uv_loop_t * loop, CMAP_MAP * job);
} CMAP_SCHEDULER_PUBLIC;

extern const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public;

#endif
