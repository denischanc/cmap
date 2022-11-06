#ifndef __CMAP_PROTOTYPE_INT_H__
#define __CMAP_PROTOTYPE_INT_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_INT_PUBLIC;

extern const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public;

#endif
