#ifndef __CMAP_PROTOTYPE_INT_H__
#define __CMAP_PROTOTYPE_INT_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  void (*require)(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx);
  void (*init)(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx);
} CMAP_PROTOTYPE_INT_PUBLIC;

extern const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public;

#endif
