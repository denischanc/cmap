#ifndef __CMAP_PROTOTYPE_LIST_H__
#define __CMAP_PROTOTYPE_LIST_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*init)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_LIST_PUBLIC;

extern const CMAP_PROTOTYPE_LIST_PUBLIC cmap_prototype_list_public;

#endif
