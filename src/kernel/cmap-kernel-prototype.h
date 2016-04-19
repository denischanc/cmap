#ifndef __CMAP_KERNEL_PROTOTYPE_H__
#define __CMAP_KERNEL_PROTOTYPE_H__

#include "cmap-map.h"

typedef struct
{
  CMAP_MAP * map_, * list_, * fn_, * string_;
} CMAP_KERNEL_PROTOTYPE;

void cmap_kernel_prototype_create();
void cmap_kernel_prototype_delete();

#endif
