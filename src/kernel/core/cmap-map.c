
#include "cmap-map.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAP_NATURE = "cmap.nature.map";

/*******************************************************************************
*******************************************************************************/

typedef struct CMAP_MAP_ENTRY_s CMAP_MAP_ENTRY;

struct CMAP_MAP_ENTRY_s
{
  const char * key_;
  CMAP_MAP * val_;

  CMAP_MAP_ENTRY * ge_, * lt_, * parent_;
};

typedef struct
{
  CMAP_MAP_ENTRY * entry_tree_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

/*******************************************************************************
*******************************************************************************/

static const char * map__nature(CMAP_MAP * this)
{
  return CMAP_MAP_NATURE;
}

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

  map -> nature = map__nature;
  map -> delete = map__delete;
  map -> add = map__add;
}

void cmap_map_delete(CMAP_MAP * map)
{
  CMAP_FREE(map -> internal_);
  CMAP_FREE(map);
}
