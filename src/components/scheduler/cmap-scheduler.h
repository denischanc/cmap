#ifndef __CMAP_SCHEDULER_H__
#define __CMAP_SCHEDULER_H__

#include <uv.h>
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

typedef struct
{
  void (*on_schedule)(uv_work_t * req, int status);

  CMAP_MAP * (*schedule)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
} CMAP_SCHEDULER_PUBLIC;

extern const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public;

#endif
