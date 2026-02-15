#ifndef __CMAP_SCHEDULER_H__
#define __CMAP_SCHEDULER_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

CMAP_MAP * cmap_scheduler_schedule_ms_fn(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);
CMAP_MAP * cmap_scheduler_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args);

#endif
