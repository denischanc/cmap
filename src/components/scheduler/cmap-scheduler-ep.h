#ifndef __CMAP_SCHEDULER_EP_H__
#define __CMAP_SCHEDULER_EP_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

typedef struct
{
  CMAP_MAP * (*schedule)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
} CMAP_SCHEDULER_EP_PUBLIC;

extern const CMAP_SCHEDULER_EP_PUBLIC cmap_scheduler_ep_public;

#endif
