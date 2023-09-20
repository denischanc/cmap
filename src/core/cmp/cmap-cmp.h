#ifndef __CMAP_CMP_H__
#define __CMAP_CMP_H__

#include "cmap-cmp-define.h"

typedef struct
{
  CMAP_CMP_LOOP(CMAP_CMP_DECL)

  int (*cmp)(CMAP_MAP * map_l, CMAP_MAP * map_r);
} CMAP_CMP_PUBLIC;

extern const CMAP_CMP_PUBLIC cmap_cmp_public;

#endif
