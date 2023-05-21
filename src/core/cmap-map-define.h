#ifndef __CMAP_MAP_DEFINE_H__
#define __CMAP_MAP_DEFINE_H__

#include "cmap.h"

#define CMAP_MAP(proc_ctx, aisle) cmap_map_public.create(proc_ctx, aisle)

#define CMAP_NATURE(map) CMAP_CALL((CMAP_MAP *)map, nature)
#define CMAP_SET(map, key, val) \
  CMAP_CALL_ARGS((CMAP_MAP *)map, set, key, (CMAP_MAP *)val)
#define CMAP_GET(map, key) CMAP_CALL_ARGS((CMAP_MAP *)map, get, key)

#define CMAP_PROTOTYPE_NEW(proto, struct, proc_ctx, aisle) \
  (struct *)CMAP_CALL_ARGS(proto, new, sizeof(struct), proc_ctx, aisle)

#endif
