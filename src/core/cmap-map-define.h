#ifndef __CMAP_MAP_DEFINE_H__
#define __CMAP_MAP_DEFINE_H__

#include "cmap.h"

#define CMAP_MAP(proc_ctx) cmap_map_public.create(proc_ctx)

#define CMAP_SET(map, key, val) \
  CMAP_CALL_ARGS((CMAP_MAP *)map, set, key, (CMAP_MAP *)val)
#define CMAP_GET(map, key) CMAP_CALL_ARGS((CMAP_MAP *)map, get, key)

#define CMAP_PROTOTYPE_NEW(proto, proc_ctx) \
  CMAP_CALL_ARGS(proto, new, proc_ctx)

#define CMAP_IS_GHOST(map) CMAP_CALL((CMAP_MAP *)map, is_ghost)

#endif
