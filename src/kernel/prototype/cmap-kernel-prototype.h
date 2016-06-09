#ifndef __CMAP_KERNEL_PROTOTYPE_H__
#define __CMAP_KERNEL_PROTOTYPE_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * map_, * list_, * fn_, * string_, * int_;
} CMAP_KERNEL_PROTOTYPE;

void cmap_prototype_init(CMAP_KERNEL_PROTOTYPE * prototype);

#endif
