#ifndef __CMAP_PART_DIRTY_H__
#define __CMAP_PART_DIRTY_H__

#include "cmap-part-ctx.h"

typedef struct
{
  void (*name)(const char * map, const char * name, CMAP_PART_CTX * ctx);
} CMAP_PART_DIRTY_PUBLIC;

extern const CMAP_PART_DIRTY_PUBLIC cmap_part_dirty_public;

#endif
