
#include "cmap-map.h"

#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-stree.h"
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
  CMAP_STREE_NODE node;

  char * key;
  CMAP_MAP * val;
} ENTRY;

typedef struct
{
  ENTRY * entry_stree;

  CMAP_MAP * prototype;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAP_NATURE = "map";

/*******************************************************************************
*******************************************************************************/

static int entry_eval(void * node, void * data)
{
  const char * key = (const char *)data;
  return strcmp(((ENTRY *)node) -> key, key);
}

CMAP_STREE_RUNNER(entry, entry_eval, NULL, CMAP_F, CMAP_F);

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
  if(*val != NULL)
  {
    CMAP_SLIST_LC_PTR * list = (CMAP_SLIST_LC_PTR *)data;
    CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)val);
  }
}

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
  CMAP_CALL_ARGS((CMAP_MAP *)this, apply, nested_apply, list);

  cmap_lifecycle_public.nested(this, list);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * set(CMAP_MAP * this, const char * key, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_STREE_FINDFN(entry, internal -> entry_stree, key);
  if(entry == NULL)
  {
    entry = CMAP_KERNEL_ALLOC(ENTRY);
    entry -> key = cmap_util_public.strdup(key);
    entry -> val = NULL;

    CMAP_STREE_ADDFN(entry, &internal -> entry_stree, entry, key);
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
  CMAP_MAP * ret = NULL;
  INTERNAL * internal = (INTERNAL *)this -> internal;

  ENTRY * entry = CMAP_STREE_FINDFN(entry, internal -> entry_stree, key);
  if(entry != NULL) ret = entry -> val;
  else
  {
    CMAP_MAP * prototype = internal -> prototype;
    if(prototype != NULL) ret = CMAP_GET(prototype, key);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * init(CMAP_MAP * this, CMAP_INITARGS * initargs);

static CMAP_MAP * new(CMAP_MAP * this, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = (CMAP_MAP *)CMAP_KERNEL_MEM -> alloc(sizeof(CMAP_MAP));

  CMAP_INITARGS initargs;
  initargs.prototype = this;
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;
  return init(map, &initargs);
}

/*******************************************************************************
*******************************************************************************/

static char is_key(CMAP_MAP * this, const char * key)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  return (CMAP_STREE_FINDFN(entry, internal -> entry_stree, key) != NULL);
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

static void apply_apply(void * node, char is_eq, void * data)
{
  ENTRY * entry = (ENTRY *)node;
  APPLY_APPLY_DATA * data_ = (APPLY_APPLY_DATA *)data;

  CMAP_MAP * prev_val = entry -> val;
  data_ -> fn(entry -> key, &entry -> val, data_ -> data);
  if(prev_val != entry -> val)
  {
    if(prev_val != NULL) CMAP_DEC_REFS(prev_val);
    if(entry -> val != NULL) CMAP_INC_REFS(entry -> val);
  }
}

CMAP_STREE_APPLY(apply_apply_stree, NULL, apply_apply, NULL);

static void apply(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  APPLY_APPLY_DATA data_;
  data_.fn = fn;
  data_.data = data;

  CMAP_STREE_APPLYFN(entry, internal -> entry_stree, apply_apply_stree, CMAP_T,
    CMAP_F, &data_);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * this;
  CMAP_MEM * mem;
} DELETE_APPLY_DATA;

static void delete_apply(void * node, char is_eq, void * data)
{
  ENTRY * entry = (ENTRY *)node;
  DELETE_APPLY_DATA * data_ = (DELETE_APPLY_DATA *)data;
  CMAP_MEM * mem = data_ -> mem;

  CMAP_MEM_FREE(entry -> key, mem);

  CMAP_MAP * val = entry -> val;
  if(val != NULL)
  {
    CMAP_LIFECYCLE * this = data_ -> this;
    cmap_log_public.debug("[%p][%s]-nested->[[%p]==>refs--]",
      this, CMAP_NATURE(this), val);
    CMAP_DEC_REFS(val);
  }

  CMAP_MEM_FREE(entry, mem);
}

static void delete(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = (INTERNAL *)((CMAP_MAP *)this) -> internal;
  CMAP_MEM * mem = CMAP_KERNEL_MEM;

  DELETE_APPLY_DATA data;
  data.this = this;
  data.mem = mem;
  CMAP_STREE_QUICKAPPLYFN(entry, internal -> entry_stree, delete_apply, &data);

  CMAP_MEM_FREE(internal, mem);

  cmap_lifecycle_public.delete(this);
}

static CMAP_MAP * init(CMAP_MAP * this, CMAP_INITARGS * initargs)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  cmap_lifecycle_public.init(lc, initargs);
  lc -> delete = delete;
  lc -> nature = nature;
  lc -> nested = nested;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> entry_stree = NULL;
  internal -> prototype = initargs -> prototype;

  this -> internal = internal;
  this -> set = set;
  this -> get = get;
  this -> new = new;
  this -> is_key = is_key;
  this -> keys = keys;
  this -> apply = apply;

  return this;
}

static CMAP_MAP * create_root(CMAP_PROC_CTX * proc_ctx)
{
  return new(NULL, proc_ctx);
}

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  return new(CMAP_CALL_ARGS(ps, map_, proc_ctx), proc_ctx);
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
