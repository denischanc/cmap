
#include "cmap-map.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-tree.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAP_NATURE = "cmap.nature.map";

/*******************************************************************************
*******************************************************************************/

typedef struct CMAP_MAP_ENTRY_s CMAP_MAP_ENTRY;

struct CMAP_MAP_ENTRY_s
{
  char * key_;
  CMAP_MAP * val_;

  CMAP_TREE_STRUCT;
};

typedef struct
{
  CMAP_MAP_ENTRY * entry_tree_;

  CMAP_MAP * prototype_, * next_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static int CMAP_TREE_EVALFN_NAME(entry)(CMAP_TREE_RUNNER * this, void * node)
{
  const char * key = (const char *)this -> internal_;
  return strcmp(((CMAP_MAP_ENTRY *)node) -> key_, key);
}

CMAP_TREE_RUNNER(CMAP_MAP_ENTRY, entry, false, false)

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
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  CMAP_TREE_RUNNER_INIT(entry, (void *)key)
  CMAP_MAP_ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree_);
  if(entry == NULL)
  {
    CMAP_MEM * mem = cmap_kernel() -> mem_;
    entry = (CMAP_MAP_ENTRY *)mem -> alloc(sizeof(CMAP_MAP_ENTRY));
    entry -> key_ = (char *)mem -> alloc((strlen(key) + 1) * sizeof(char));
    strcpy(entry -> key_, key);

    CMAP_TREE_ADDFN(entry, &internal -> entry_tree_, entry);
  }
  entry -> val_ = val;
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
  CMAP_INTERNAL * internal = CMAP_ALLOC_STRUCT(CMAP_INTERNAL);
  internal -> entry_tree_ = NULL;
  internal -> prototype_ = NULL;
  internal -> next_ = NULL;

  map -> internal_ = internal;
  map -> nature = map__nature;
  map -> delete = map__delete;
  map -> add = map__add;
}

void cmap_map_delete(CMAP_MAP * map)
{
  // TODO : CMAP_TREE_APPLY
  CMAP_FREE(map -> internal_);
  CMAP_FREE(map);
}
