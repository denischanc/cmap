
#include "cmap-map.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAP_NATURE = "cmap.nature.map";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * map__nature(CMAP_ROOT * this)
{
  return CMAP_MAP_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void map__delete(CMAP_ROOT * this)
{
  cmap_map_delete((CMAP_MAP *)this);
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
  CMAP_ROOT * root = (CMAP_ROOT *)map;
  cmap_root_init(root);
  root -> nature = map__nature;
  root -> delete = map__delete;

  map -> internal_ = CMAP_ALLOC_STRUCT(CMAP_INTERNAL);
  map -> add = map__add;
}

void cmap_map_delete(CMAP_MAP * map)
{
  CMAP_FREE(map -> internal_);
  cmap_root_delete((CMAP_ROOT *)map);
}
