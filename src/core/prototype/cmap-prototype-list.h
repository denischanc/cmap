#ifndef __CMAP_PROTOTYPE_LIST_H__
#define __CMAP_PROTOTYPE_LIST_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_LIST_PUBLIC;

extern const CMAP_PROTOTYPE_LIST_PUBLIC cmap_prototype_list_public;

#endif
