#ifndef __CMAP_SCHEDULER_EP_H__
#define __CMAP_SCHEDULER_EP_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

CMAP_MAP * cmap_scheduler_ep_public_schedule(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);

#endif
