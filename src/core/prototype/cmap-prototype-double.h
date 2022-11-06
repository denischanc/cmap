#ifndef __CMAP_PROTOTYPE_DOUBLE_H__
#define __CMAP_PROTOTYPE_DOUBLE_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_DOUBLE_PUBLIC;

extern const CMAP_PROTOTYPE_DOUBLE_PUBLIC cmap_prototype_double_public;

#endif
