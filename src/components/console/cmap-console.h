#ifndef __CMAP_CONSOLE_H__
#define __CMAP_CONSOLE_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"

typedef struct
{
  CMAP_MAP * (*info)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
  CMAP_MAP * (*print)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
  CMAP_MAP * (*error)(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
    CMAP_LIST * args);
} CMAP_CONSOLE_PUBLIC;

extern const CMAP_CONSOLE_PUBLIC cmap_console_public;

#endif
