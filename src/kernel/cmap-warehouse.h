#ifndef __CMAP_WAREHOUSE_H__
#define __CMAP_WAREHOUSE_H__

#include "cmap-map.h"

typedef struct CMAP_WAREHOUSE_s CMAP_WAREHOUSE;

struct CMAP_WAREHOUSE_s
{
  CMAP_MAP super_;

  void (*delete)(CMAP_WAREHOUSE * this, const char * aisle);
  void (*delete_last)(CMAP_WAREHOUSE * this, const char * aisle);
};

CMAP_WAREHOUSE * cmap_warehouse_create();

#endif
