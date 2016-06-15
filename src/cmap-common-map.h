#ifndef __CMAP_COMMON_MAP_H__
#define __CMAP_COMMON_MAP_H__

#include "cmap-map.h"

#define CMAP_NATURE(map) \
  CMAP_CALL(map, nature)

#define CMAP_DELETE(map) \
  CMAP_CALL((CMAP_MAP *)map, delete)

#define CMAP_SET(map, key, val) \
  CMAP_CALL_ARGS((CMAP_MAP *)map, set, key, (CMAP_MAP *)(val))

#define CMAP_GET(map, key) \
  CMAP_CALL_ARGS((CMAP_MAP *)map, get, key)

#define CMAP_NEW_MAP(prototype, aisle) \
  (CMAP_MAP *)CMAP_CALL_ARGS( \
    (CMAP_MAP *)prototype, new, sizeof(CMAP_MAP), aisle)

#endif
