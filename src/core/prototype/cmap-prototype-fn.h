#ifndef __CMAP_PROTOTYPE_FN_H__
#define __CMAP_PROTOTYPE_FN_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_FN_PUBLIC;

extern const CMAP_PROTOTYPE_FN_PUBLIC cmap_prototype_fn_public;

#endif
