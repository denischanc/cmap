#ifndef __CMAP_CMP_H__
#define __CMAP_CMP_H__

#include "cmap-map-type.h"

typedef struct
{
  int (*cmp)(CMAP_MAP * map_l, CMAP_MAP * map_r);
} CMAP_CMP_PUBLIC;

extern const CMAP_CMP_PUBLIC cmap_cmp_public;

#endif
