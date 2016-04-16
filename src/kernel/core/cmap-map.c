
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

static void map_init(CMAP_MAP * map);

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

void cmap_map__set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
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

static void entry_delete(CMAP_TREE_APPLY * this, void ** node)
{
  CMAP_MEM * mem = (CMAP_MEM *)this -> internal_;
  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)*node;
  CMAP_FREE(entry -> key_, mem);
  CMAP_FREE(entry, mem);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map__get(CMAP_MAP * this, const char * key)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  CMAP_TREE_RUNNER_INIT(entry, (void *)key)
  CMAP_MAP_ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree_);
  if(entry != NULL) return entry -> val_;
  else
  {
    CMAP_MAP * prototype = internal -> prototype_;
    if(prototype != NULL) return CMAP_CALL_ARGS(prototype, get, key);
    else return NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

void * cmap_map__new(CMAP_MAP * this, int size)
{
  CMAP_MAP * map = (CMAP_MAP *)cmap_kernel() -> mem_ -> alloc(size);
  map_init(map);
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)map -> internal_;
  internal -> prototype_ = this;
  return (void *)map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_create()
{
  CMAP_MAP * prototype_map = cmap_kernel() -> prototype_.map_;
  if(prototype_map == NULL) return cmap_root_map_create();
  else return CMAP_NEW_MAP(prototype_map);
}

CMAP_MAP * cmap_root_map_create()
{
  CMAP_KERNEL_ALLOC_PTR(map, CMAP_MAP);
  map_init(map);
  return map;
}

static void map_init(CMAP_MAP * map)
{
  CMAP_KERNEL_ALLOC_PTR(internal, CMAP_INTERNAL);
  internal -> entry_tree_ = NULL;
  internal -> prototype_ = NULL;
  internal -> next_ = NULL;

  map -> internal_ = internal;
  map -> nature = map__nature;
  map -> delete = map__delete;
  map -> set = cmap_map__set;
  map -> get = cmap_map__get;
  map -> new = cmap_map__new;
}

void cmap_map_delete(CMAP_MAP * map)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)map -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, mem, NULL, NULL, entry_delete)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree_, apply, CMAP_T);

  CMAP_FREE(internal, mem);
  CMAP_FREE(map, mem);
}
