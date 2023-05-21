#ifndef __CMAP_AISLESTORE_H__
#define __CMAP_AISLESTORE_H__

#include "cmap-map.h"

typedef struct CMAP_AISLESTORE CMAP_AISLESTORE;

struct CMAP_AISLESTORE
{
  CMAP_MAP super;

  void (*delete)(CMAP_AISLESTORE * this, const char * aisle);
  void (*delete_last)(CMAP_AISLESTORE * this, const char * aisle);
};

typedef struct
{
  CMAP_AISLESTORE * (*create)();
} CMAP_AISLESTORE_PUBLIC;

extern const CMAP_AISLESTORE_PUBLIC cmap_aislestore_public;

#endif
