#ifndef __CMAP_PROTOTYPE_PTR_H__
#define __CMAP_PROTOTYPE_PTR_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*require)();
  CMAP_MAP * (*instance)();
} CMAP_PROTOTYPE_PTR_PUBLIC;

extern const CMAP_PROTOTYPE_PTR_PUBLIC cmap_prototype_ptr_public;

#endif
