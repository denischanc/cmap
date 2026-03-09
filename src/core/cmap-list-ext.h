#ifndef __CMAP_LIST_EXT_H__
#define __CMAP_LIST_EXT_H__

#include "cmap-list-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

CMAP_LIST * cmap_list_set(CMAP_LIST * list, int i, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx);
CMAP_MAP * cmap_list_get(CMAP_LIST * list, int i);

extern const char * CMAP_LIST_NATURE;

#endif
