#ifndef __CMAP_PROTOTYPE_MAP_H__
#define __CMAP_PROTOTYPE_MAP_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_MAP_PUBLIC;

extern const CMAP_PROTOTYPE_MAP_PUBLIC cmap_prototype_map_public;

#endif
