#ifndef __CMAP_POOL_HANDLER_DEFINE_H__
#define __CMAP_POOL_HANDLER_DEFINE_H__

#include "cmap-list-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_POOL_HANDLER_LOOP(macro) \
  macro(LIST_GHOST, list_ghost, CMAP_LIST *) \
  macro(MAP_GHOST, map_ghost, CMAP_MAP *)

#define CMAP_POOL_HANDLER_DECL(NAME, name, type) \
  type cmap_pool_handler_##name##_create(CMAP_PROC_CTX * proc_ctx); \
  void cmap_pool_handler_##name##_clean(type e);

#endif
