
#include "cmap-map.h"

#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-tree.h"
#include "cmap-prototype-map.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-util.h"

/* TODO : annotations */

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_TREE_NODE node;

  char * key;
  CMAP_MAP * val;
} ENTRY;

typedef struct
{
  ENTRY * entry_tree;

  CMAP_MAP * prototype;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAP_NATURE = "map";

/*******************************************************************************
*******************************************************************************/

static int CMAP_TREE_EVALFN_NAME(entry)(CMAP_TREE_RUNNER * this, void * node,
  void * data)
{
  const char * key = (const char *)data;
  return strcmp(((ENTRY *)node) -> key, key);
}

CMAP_TREE_RUNNER(entry, NULL, false, false);

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_MAP_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree, key);
  if(entry == NULL)
  {
    entry = CMAP_KERNEL_ALLOC(ENTRY);
    entry -> key = cmap_util_public.strdup(key);

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

static void init(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx, const char * aisle);

static void * new(CMAP_MAP * this, int size, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_MAP * map = (CMAP_MAP *)cmap_kernel_public.mem() -> alloc(size);
  init(map, proc_ctx, aisle);
  INTERNAL * internal = (INTERNAL *)map -> internal;
  internal -> prototype = this;
  return map;
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
  CMAP_PROC_CTX * proc_ctx;
  const char * aisle;
} KEYS_DATA;

static void add_key(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  KEYS_DATA * data_ = (KEYS_DATA *)data;
  CMAP_STRING * key =
    CMAP_STRING(((ENTRY *)*node) -> key, 0, data_ -> proc_ctx, data_ -> aisle);
  CMAP_LIST_ADD(data_ -> keys, 0, key);
}

CMAP_TREE_APPLY(keys_apply, NULL, NULL, add_key, NULL);

static void keys(CMAP_MAP * this, CMAP_LIST * keys, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  KEYS_DATA data;
  data.keys = keys;
  data.proc_ctx = proc_ctx;
  data.aisle = aisle;

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, keys_apply, CMAP_T, &data);
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

CMAP_TREE_APPLY(apply_apply, NULL, NULL, apply_fn, NULL);

static void apply(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  APPLY_DATA apply_data;
  apply_data.fn = fn;
  apply_data.fn_data = data;

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, apply_apply, CMAP_T,
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

CMAP_TREE_APPLY(delete_apply, NULL, NULL, NULL, entry_delete);

static CMAP_LIFECYCLE * delete(CMAP_MAP * map)
{
  INTERNAL * internal = (INTERNAL *)map -> internal;
  CMAP_MEM * mem = cmap_kernel_public.mem();

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, delete_apply, CMAP_T, mem);

  CMAP_MEM_FREE(internal, mem);

  return cmap_lifecycle_public.delete(&map -> super);
}

static CMAP_LIFECYCLE * delete_(CMAP_LIFECYCLE * map)
{
  return delete((CMAP_MAP *)map);
}

static void init(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_LIFECYCLE * lc = &map -> super;
  cmap_lifecycle_public.init(lc, proc_ctx, aisle);
  lc -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> entry_tree = NULL;
  internal -> prototype = NULL;

  map -> internal = internal;
  map -> nature = nature;
  map -> set = set;
  map -> get = get;
  map -> new = new;
  map -> is_key = is_key;
  map -> keys = keys;
  map -> apply = apply;
}

static CMAP_MAP * create_root(CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  return (CMAP_MAP *)new(NULL, sizeof(CMAP_MAP), proc_ctx, aisle);
}

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * prototype_map = cmap_prototype_map_public.instance(proc_ctx);
  return CMAP_PROTOTYPE_NEW(prototype_map, CMAP_MAP, proc_ctx, aisle);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_MAP_PUBLIC cmap_map_public =
{
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
