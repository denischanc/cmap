
#include "cmap-map.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-tree.h"
#include "cmap-fw.h"

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

static int CMAP_TREE_EVALFN_NAME(entry)(CMAP_TREE_RUNNER * this, void * node,
  void * data)
{
  const char * key = (const char *)data;
  return strcmp(((CMAP_MAP_ENTRY *)node) -> key_, key);
}

CMAP_TREE_RUNNER(CMAP_MAP_ENTRY, entry, NULL, false, false)

/*******************************************************************************
*******************************************************************************/

static void fill_warehouse(const char * aisle, CMAP_MAP * map)
{
  CMAP_MAP * wh = (CMAP_MAP *)cmap_kernel() -> warehouse_;

  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)map -> internal_;
  internal -> next_ = CMAP_CALL_ARGS(wh, get, aisle);

  CMAP_CALL_ARGS(wh, set, aisle, map);
}

/*******************************************************************************
*******************************************************************************/

static const char * map__nature(CMAP_MAP * this)
{
  return CMAP_MAP_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * map__delete(CMAP_MAP * this)
{
  return cmap_map_delete(this);
}

/*******************************************************************************
*******************************************************************************/

void cmap_map__set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  CMAP_MAP_ENTRY * entry =
    CMAP_TREE_FINDFN(entry, internal -> entry_tree_, key);
  if(entry == NULL)
  {
    CMAP_MEM * mem = cmap_kernel() -> mem_;
    entry = (CMAP_MAP_ENTRY *)mem -> alloc(sizeof(CMAP_MAP_ENTRY));
    entry -> key_ = (char *)mem -> alloc((strlen(key) + 1) * sizeof(char));
    strcpy(entry -> key_, key);

    CMAP_TREE_ADDFN(entry, &internal -> entry_tree_, entry, key);
  }
  entry -> val_ = val;
}

/*******************************************************************************
*******************************************************************************/

static void entry_delete(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  CMAP_MEM * mem = (CMAP_MEM *)data;
  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)*node;
  CMAP_FREE(entry -> key_, mem);
  CMAP_FREE(entry, mem);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map__get(CMAP_MAP * this, const char * key)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  CMAP_MAP_ENTRY * entry =
    CMAP_TREE_FINDFN(entry, internal -> entry_tree_, key);
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

void * cmap_map__new(CMAP_MAP * this, int size, const char * aisle)
{
  CMAP_MAP * map = (CMAP_MAP *)cmap_kernel() -> mem_ -> alloc(size);
  cmap_map_init(map);
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)map -> internal_;
  internal -> prototype_ = this;
  if(aisle != NULL) fill_warehouse(aisle, map);
  return (void *)map;
}

/*******************************************************************************
*******************************************************************************/

char cmap_map__is_key(CMAP_MAP * this, const char * key)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  return (CMAP_TREE_FINDFN(entry, internal -> entry_tree_, key) != NULL);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST * keys_;
  const char * aisle_;
} KEYS_DATA;

static void add_key(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  KEYS_DATA * _data = (KEYS_DATA *)data;
  CMAP_STRING * key =
    CMAP_STRING(((CMAP_MAP_ENTRY *)*node) -> key_, 0, _data -> aisle_);
  CMAP_CALL_ARGS(_data -> keys_, add, 0, (CMAP_MAP *)key);
}

void cmap_map__keys(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;

  KEYS_DATA data;
  data.keys_ = keys;
  data.aisle_ = aisle;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, add_key, NULL)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree_, apply, CMAP_T, &data);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_create(const char * aisle)
{
  CMAP_MAP * prototype_map = cmap_kernel() -> prototype_.map_;
  if(prototype_map == NULL) return cmap_root_map_create(aisle);
  else return CMAP_NEW_MAP(prototype_map, aisle);
}

CMAP_MAP * cmap_root_map_create(const char * aisle)
{
  CMAP_KERNEL_ALLOC_PTR(map, CMAP_MAP);
  cmap_map_init(map);
  if(aisle != NULL) fill_warehouse(aisle, map);
  return map;
}

void cmap_map_init(CMAP_MAP * map)
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
  map -> is_key = cmap_map__is_key;
  map -> keys = cmap_map__keys;
}

CMAP_MAP * cmap_map_delete(CMAP_MAP * map)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)map -> internal_;
  CMAP_MAP * next = internal -> next_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, NULL, entry_delete)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree_, apply, CMAP_T, mem);

  CMAP_FREE(internal, mem);
  CMAP_FREE(map, mem);

  return next;
}
