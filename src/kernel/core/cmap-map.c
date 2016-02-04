
#include "cmap-map.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void map__delete(CMAP_MAP * this)
{
  cmap_map_delete(this);
}

/*******************************************************************************
*******************************************************************************/

static void map__add(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_create()
{
  CMAP_MAP * map = CMAP_ALLOC_STRUCT(CMAP_MAP);
  cmap_map_init(map);
  return map;
}

void cmap_map_init(CMAP_MAP * map)
{
  map -> internal_ = CMAP_ALLOC_STRUCT(CMAP_INTERNAL);

  map -> delete = map__delete;
  map -> add = map__add;
}

void cmap_map_delete(CMAP_MAP * map)
{
  CMAP_FREE(map -> internal_);
  CMAP_FREE(map);
}
