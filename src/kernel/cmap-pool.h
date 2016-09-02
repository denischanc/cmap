#ifndef __CMAP_POOL_H__
#define __CMAP_POOL_H__

#include "cmap-map.h"

typedef struct CMAP_POOL_s CMAP_POOL;

struct CMAP_POOL_s
{
  void * internal_;

  void (*delete)(CMAP_POOL * this);

  CMAP_MAP * (*take)(CMAP_POOL * this);
  void (*release)(CMAP_POOL * this, CMAP_MAP * map);
};

typedef struct
{
  CMAP_MAP * (*create)();
  void (*delete)(CMAP_MAP * map);
  void (*clean)(CMAP_MAP * map);
} CMAP_POOL_HANDLER;

CMAP_POOL * cmap_pool_create(CMAP_POOL_HANDLER * handler, int size);

#endif
