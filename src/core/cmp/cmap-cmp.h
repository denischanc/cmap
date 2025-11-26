#ifndef __CMAP_CMP_H__
#define __CMAP_CMP_H__

#include <stdint.h>
#include "cmap-map-type.h"

typedef struct
{
  int64_t (*cmp)(CMAP_MAP * map_l, CMAP_MAP * map_r);
} CMAP_CMP_PUBLIC;

extern const CMAP_CMP_PUBLIC cmap_cmp_public;

#endif
