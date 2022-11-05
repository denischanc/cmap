#ifndef __CMAP_PROTOTYPE_UTIL__
#define __CMAP_PROTOTYPE_UTIL__

#include "cmap-prototype-util-define.h"
#include "cmap-map.h"
#include "cmap-fn.h"
#include "cmap-list.h"

typedef struct
{
  CMAP_MAP * map;
  CMAP_FN * fn;
} CMAP_PROTOTYPE_UTIL_MAP_FN;

typedef struct
{
  char (*args_to_map_fn)(CMAP_LIST * args, CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn);
} CMAP_PROTOTYPE_UTIL_PUBLIC;

extern const CMAP_PROTOTYPE_UTIL_PUBLIC cmap_prototype_util_public;

#endif
