
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
#include "cmap-log.h"

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

static const char * nature(CMAP_LIFECYCLE * this)
{
  return CMAP_MAP_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void nested_apply(const char * key, CMAP_MAP ** val, void * data)
{
  CMAP_STACK_lc_ptr ** stack = (CMAP_STACK_lc_ptr **)data;
  cmap_stack_lc_ptr_public.push(stack, (CMAP_LIFECYCLE **)val);
}

static void nested(CMAP_LIFECYCLE * this, CMAP_STACK_lc_ptr ** stack)
{
  CMAP_CALL_ARGS((CMAP_MAP *)this, apply, nested_apply, stack);

  cmap_lifecycle_public.nested(this, stack);
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

  if(entry -> val != NULL) CMAP_DEC_REFS(entry -> val);
  entry -> val = val;
  if(val != NULL) CMAP_INC_REFS(val);

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
} KEYS_APPLY_DATA;

static void keys_apply(const char * key, CMAP_MAP ** val, void * data)
{
  KEYS_APPLY_DATA * data_ = (KEYS_APPLY_DATA *)data;
  CMAP_LIST_PUSH(data_ -> keys, CMAP_STRING(key, 0, data_ -> proc_ctx));
}

static CMAP_LIST * keys(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx)
{
  KEYS_APPLY_DATA data;
  data.keys = CMAP_LIST(0, proc_ctx);
  data.proc_ctx = proc_ctx;

  CMAP_CALL_ARGS(this, apply, keys_apply, &data);

  return data.keys;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_MAP_ENTRY_FN fn;
  void * data;
} APPLY_APPLY_DATA;

static void apply_apply(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  ENTRY * entry = (ENTRY *)*node;
  APPLY_APPLY_DATA * data_ = (APPLY_APPLY_DATA *)data;

  CMAP_MAP * prev_val = entry -> val;
  data_ -> fn(entry -> key, &entry -> val, data_ -> data);
  if(prev_val != entry -> val)
  {
    if(prev_val != NULL) CMAP_DEC_REFS(prev_val);
    if(entry -> val != NULL) CMAP_INC_REFS(entry -> val);
  }
}

CMAP_TREE_APPLY(apply_apply_tree, NULL, NULL, apply_apply, NULL);

static void apply(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  APPLY_APPLY_DATA data_;
  data_.fn = fn;
  data_.data = data;

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, apply_apply_tree, CMAP_T,
    &data_);
}

/*******************************************************************************
*******************************************************************************/

static void delete_apply(CMAP_TREE_APPLY * this, void ** node, void * data)
{
  CMAP_MEM * mem = (CMAP_MEM *)data;
  ENTRY * entry = (ENTRY *)*node;
  CMAP_MEM_FREE(entry -> key, mem);
  CMAP_MEM_FREE(entry, mem);
}

CMAP_TREE_APPLY(delete_apply_tree, NULL, NULL, NULL, delete_apply);

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  INTERNAL * internal = (INTERNAL *)((CMAP_MAP *)this) -> internal;
  CMAP_MEM * mem = cmap_kernel_public.mem();

  CMAP_TREE_APPLYFN(entry, &internal -> entry_tree, delete_apply_tree, CMAP_T,
    mem);

  CMAP_MEM_FREE(internal, mem);

  cmap_lifecycle_public.delete(this);
}

static void init(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  cmap_lifecycle_public.init(lc, proc_ctx);
  lc -> delete = delete;
  lc -> nature = nature;
  lc -> nested = nested;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> entry_tree = NULL;
  internal -> prototype = NULL;

  this -> internal = internal;
  this -> set = set;
  this -> get = get;
  this -> new = new;
  this -> is_key = is_key;
  this -> keys = keys;
  this -> apply = apply;
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
  create, create_root, init, delete,
  nested,
  set,
  get,
  new,
  is_key, keys,
  apply
};
