#ifndef __CMAP_SCHEDULER_H__
#define __CMAP_SCHEDULER_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_SCHEDULER_PUBLIC;

extern const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public;

#endif
