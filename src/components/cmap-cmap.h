#ifndef __CMAP_CMAP_H__
#define __CMAP_CMAP_H__

#include <cmap/core.h>

typedef struct
{
  CMAP_MAP * (*create)();
} CMAP_CMAP_PUBLIC;

extern const CMAP_CMAP_PUBLIC cmap_cmap_public;

#endif
