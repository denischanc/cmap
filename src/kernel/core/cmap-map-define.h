#ifndef __CMAP_MAP_DEFINE_H__
#define __CMAP_MAP_DEFINE_H__

#include "cmap-common-define.h"

#define CMAP_MAP_NEW_MAP(prototype, aisle) (CMAP_MAP *)CMAP_CALL_ARGS( \
  (CMAP_MAP *)(prototype), new, sizeof(CMAP_MAP), aisle)

#endif
