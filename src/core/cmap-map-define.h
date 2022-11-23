#ifndef __CMAP_MAP_DEFINE_H__
#define __CMAP_MAP_DEFINE_H__

#include "cmap-common.h"

#define CMAP_MAP_NEW_MAP(prototype, proc_ctx, aisle) \
  (CMAP_MAP *)CMAP_CALL_ARGS( \
    (CMAP_MAP *)prototype, new, sizeof(CMAP_MAP), proc_ctx, aisle)

#endif
