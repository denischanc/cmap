#ifndef __CMAP_PROTOTYPE_STRING_H__
#define __CMAP_PROTOTYPE_STRING_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * (*init)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_STRING_PUBLIC;

extern const CMAP_PROTOTYPE_STRING_PUBLIC cmap_prototype_string_public;

#endif
