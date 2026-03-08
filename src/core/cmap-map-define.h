#ifndef __CMAP_MAP_DEFINE_H__
#define __CMAP_MAP_DEFINE_H__

#include "cmap.h"

#define CMAP_MAP(proc_ctx) cmap_map_create(proc_ctx)

#define CMAP_SET(map, key, val) \
  cmap_map_set((CMAP_MAP *)map, key, (CMAP_MAP *)val)
#define CMAP_GET(map, key) cmap_map_get((CMAP_MAP *)map, key)

#define CMAP_PROTOTYPE_NEW(proto, proc_ctx) cmap_map_new(proto, proc_ctx)

#define CMAP_IS_GHOST(map) cmap_map_is_ghost((CMAP_MAP *)map)

#endif
