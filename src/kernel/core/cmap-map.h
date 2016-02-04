#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-root.h"

extern const char * CMAP_MAP_NATURE;

typedef struct CMAP_MAP_s CMAP_MAP;

struct CMAP_MAP_s
{
  CMAP_ROOT super;

  void * internal_;

  void (*add)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
};

CMAP_MAP * cmap_map_create();
void cmap_map_init(CMAP_MAP * map);
void cmap_map_delete(CMAP_MAP * map);

#endif
