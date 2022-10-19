
#include "cmap-map.h"

#include <string.h>
#include "cmap-kernel.h"
#include "cmap-tree.h"
#include "cmap.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * key;
  CMAP_MAP * val;

  CMAP_TREE_STRUCT;
} ENTRY;

typedef struct
{
  ENTRY * entry_tree;

  CMAP_MAP * prototype, * next;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static int CMAP_TREE_EVALFN_NAME(entry)(CMAP_TREE_RUNNER * this, void * node,
  void * data)
{
  const char * key = (const char *)data;
  return strcmp(((ENTRY *)node) -> key, key);
}

CMAP_TREE_RUNNER(ENTRY, entry, NULL, false, false)

/*******************************************************************************
*******************************************************************************/

static void fill_warehouse(const char * aisle, CMAP_MAP * map)
{
  CMAP_MAP * wh = (CMAP_MAP *)cmap_kernel_public.instance() -> warehouse;

  if(!strcmp(aisle, CMAP_AISLE_LOCAL))
  {
    CMAP_LIST * stack_local = (CMAP_LIST *)CMAP_GET(wh, CMAP_AISLE_STACK);
    CMAP_LIST_PUSH(stack_local, map);
  }
  else
  {
    INTERNAL * internal = (INTERNAL *)map -> internal;
    internal -> next = CMAP_GET(wh, aisle);

    CMAP_SET(wh, aisle, map);
  }
}

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return cmap_map_public.nature;
}

/*******************************************************************************
*******************************************************************************/

static void set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree, key);
  if(entry == NULL)
  {
    CMAP_MEM * mem = cmap_kernel_public.instance() -> mem;
    entry = (ENTRY *)mem -> alloc(sizeof(ENTRY));
    entry -> key = (char *)mem -> alloc((strlen(key) + 1) * sizeof(char));
    strcpy(entry -> key, key);

    CMAP_TREE_ADDFN(entry, &internal -> entry_tree, entry, key);
  }
  entry -> val = val;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * get(CMAP_MAP * this, const char * key)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree, key);
  if(entry != NULL) return entry -> val;
  else
  {
    CMAP_MAP * prototype = internal -> prototype;
    if(prototype != NULL) return CMAP_GET(prototype, key);
    else return NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_MAP * map);

static void * new(CMAP_MAP * this, int size, const char * aisle)
{
  CMAP_MAP * map =
    (CMAP_MAP *)cmap_kernel_public.instance() -> mem -> alloc(size);
  init(map);
  INTERNAL * internal = (INTERNAL *)map -> internal;
  internal -> prototype = this;
  if(aisle != NULL) fill_warehouse(aisle, map);
  return (void *)map;
}

/*******************************************************************************
*******************************************************************************/

static char is_key(CMAP_MAP * this, const char * key)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  return (CMAP_TREE_FINDFN(entry, internal -> entry_tree, key) != NULL);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST * keys;
  const char * aisle;
} KEYS_DATA;

static void add_key(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  KEYS_DATA * data_ = (KEYS_DATA *)data;
  CMAP_STRING * key =
    CMAP_STRING(((ENTRY *)*node) -> key, 0, data_ -> aisle);
  CMAP_LIST_ADD(data_ -> keys, 0, key);
}

static void keys(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  KEYS_DATA data;
  data.keys = keys;
  data.aisle = aisle;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, add_key, NULL)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, apply, CMAP_T, &data);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_MAP_ENTRY_FN fn;
  void * fn_data;
} APPLY_DATA;

static void apply_fn(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  ENTRY * entry = (ENTRY *)*node;
  APPLY_DATA * apply_data = (APPLY_DATA *)data;

  apply_data -> fn(entry -> key, &entry -> val, apply_data -> fn_data);
}

static void apply(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  APPLY_DATA apply_data;
  apply_data.fn = fn;
  apply_data.fn_data = data;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, apply_fn, NULL)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, apply, CMAP_T,
    &apply_data);
}

/*******************************************************************************
*******************************************************************************/

static void entry_delete(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  CMAP_MEM * mem = (CMAP_MEM *)data;
  ENTRY * entry = (ENTRY *)*node;
  CMAP_MEM_FREE(entry -> key, mem);
  CMAP_MEM_FREE(entry, mem);
}

static CMAP_MAP * delete(CMAP_MAP * map)
{
  INTERNAL * internal = (INTERNAL *)map -> internal;
  CMAP_MAP * next = internal -> next;
  CMAP_MEM * mem = cmap_kernel_public.instance() -> mem;

  CMAP_TREE_APPLY apply;
  CMAP_TREE_APPLY_INIT(apply, NULL, NULL, NULL, entry_delete)
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, apply, CMAP_T, mem);

  CMAP_MEM_FREE(internal, mem);
  CMAP_MEM_FREE(map, mem);

  return next;
}

static void init(CMAP_MAP * map)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> entry_tree = NULL;
  internal -> prototype = NULL;
  internal -> next = NULL;

  map -> internal = internal;
  map -> nature = nature;
  map -> delete = delete;
  map -> set = set;
  map -> get = get;
  map -> new = new;
  map -> is_key = is_key;
  map -> keys = keys;
  map -> apply = apply;
}

static CMAP_MAP * create_root(const char * aisle)
{
  CMAP_KERNEL_ALLOC_PTR(map, CMAP_MAP);
  init(map);
  if(aisle != NULL) fill_warehouse(aisle, map);
  return map;
}

static CMAP_MAP * create(const char * aisle)
{
  CMAP_MAP * prototype_map = cmap_kernel_public.instance() -> prototype.map_;
  if(prototype_map == NULL) return create_root(aisle);
  else return CMAP_MAP_NEW_MAP(prototype_map, aisle);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_MAP_PUBLIC cmap_map_public =
{
  "cmap.nature.map",
  create,
  create_root,
  init,
  delete,
  set,
  get,
  new,
  is_key,
  keys,
  apply
};
