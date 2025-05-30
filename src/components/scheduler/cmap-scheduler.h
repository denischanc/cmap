#ifndef __CMAP_SCHEDULER_H__
#define __CMAP_SCHEDULER_H__

#include "cmap-uv.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

typedef struct
{
  void (*schedule)(uv_idle_t * idle);

  CMAP_MAP * (*scheduler_empty)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
} CMAP_SCHEDULER_PUBLIC;

extern const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public;

#endif
