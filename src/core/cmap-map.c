
#include "cmap-map.h"

#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-tree.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-util.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

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

static CMAP_MAP * set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_TREE_FINDFN(entry, internal -> entry_tree, key);
  if(entry == NULL)
  {
    entry = CMAP_KERNEL_ALLOC(ENTRY);
    entry -> key = cmap_util_public.strdup(key);
    entry -> val = NULL;

    CMAP_TREE_ADDFN(entry, &internal -> entry_tree, entry, key);
  }

  if(entry -> val != NULL) CMAP_DEC_REF(entry -> val);
  entry -> val = val;
  if(val != NULL) CMAP_INC_REF(val);

  return val;
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

static void init(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx);

static void * new(CMAP_MAP * this, int size, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = (CMAP_MAP *)cmap_kernel_public.mem() -> alloc(size);
  init(map, proc_ctx);
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
} KEYS_DATA;

static void add_key(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  KEYS_DATA * data_ = (KEYS_DATA *)data;
  CMAP_STRING * key =
    CMAP_STRING(((ENTRY *)*node) -> key, 0, data_ -> proc_ctx);
  CMAP_LIST_ADD(data_ -> keys, 0, key);
}

CMAP_TREE_APPLY(keys_apply, NULL, NULL, add_key, NULL);

static void keys(CMAP_MAP * this, CMAP_LIST * keys, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  KEYS_DATA data;
  data.keys = keys;
  data.proc_ctx = proc_ctx;

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

  CMAP_MAP * prev_val = entry -> val;
  apply_data -> fn(entry -> key, &entry -> val, apply_data -> fn_data);
  if(prev_val != entry -> val)
  {
    if(prev_val != NULL) CMAP_DEC_REF(prev_val);
    if(entry -> val != NULL) CMAP_INC_REF(entry -> val);
  }
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

static void entry_dec_ref(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  CMAP_MAP * val = ((ENTRY *)*node) -> val;
  if(val != NULL) CMAP_DEC_REF(val);
}

CMAP_TREE_APPLY(dec_ref_apply, NULL, NULL, NULL, entry_dec_ref);

static void deep_delete(CMAP_MAP * map)
{
  INTERNAL * internal = (INTERNAL *)map -> internal;
  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, dec_ref_apply, CMAP_T,
    NULL);

  cmap_lifecycle_public.deep_delete(&map -> super);
}

static void deep_delete_(CMAP_LIFECYCLE * map)
{
  deep_delete((CMAP_MAP *)map);
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

static void delete(CMAP_MAP * map)
{
  INTERNAL * internal = (INTERNAL *)map -> internal;
  CMAP_MEM * mem = cmap_kernel_public.mem();

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, delete_apply, CMAP_T, mem);

  CMAP_MEM_FREE(internal, mem);

  cmap_lifecycle_public.delete(&map -> super);
}

static void delete_(CMAP_LIFECYCLE * map)
{
  delete((CMAP_MAP *)map);
}

static void init(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_LIFECYCLE * lc = &map -> super;
  cmap_lifecycle_public.init(lc, proc_ctx);
  lc -> delete = delete_;
  lc -> deep_delete = deep_delete_;

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

static CMAP_MAP * create_root(CMAP_PROC_CTX * proc_ctx)
{
  return (CMAP_MAP *)new(NULL, sizeof(CMAP_MAP), proc_ctx);
}

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_map = CMAP_CALL_ARGS(ps, map_, proc_ctx);
  return CMAP_PROTOTYPE_NEW(prototype_map, CMAP_MAP, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_MAP_PUBLIC cmap_map_public =
{
  create,
  create_root,
  init,
  delete,
  deep_delete,
  set,
  get,
  new,
  is_key,
  keys,
  apply
};
