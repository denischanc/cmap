#ifndef __CMAP_CMAP_EP_H__
#define __CMAP_CMAP_EP_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  CMAP_MAP * (*prototype)(CMAP_PROC_CTX * proc_ctx);
} CMAP_CMAP_EP_PUBLIC;

extern const CMAP_CMAP_EP_PUBLIC cmap_cmap_ep_public;

#endif
