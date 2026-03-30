
#include "cmap-map.h"

#include <string.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-stree.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-util.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"
#include "cmap-core.h"

/*******************************************************************************
*******************************************************************************/

struct CMAP_MAP_ENTRY
{
  CMAP_STREE_NODE node;

  char * key;
  CMAP_MAP * val;
};

/*******************************************************************************
*******************************************************************************/

static int64_t entry_eval(CMAP_STREE_NODE * node, void * data)
{
  const char * key = data;
  return strcmp(((CMAP_MAP_ENTRY *)node) -> key, key);
}

CMAP_STREE_RUNNER(entry, entry_eval, NULL, CMAP_F, CMAP_F);

/*******************************************************************************
*******************************************************************************/

static void nested_apply(const char * key, CMAP_MAP ** val, void * data,
  CMAP_PROC_CTX * proc_ctx)
{
  if(*val != NULL)
  {
    CMAP_SLIST_LC_PTR * list = (CMAP_SLIST_LC_PTR *)data;
    cmap_slist_lc_ptr_push(list, (CMAP_LIFECYCLE **)val);
  }
}

void cmap_map_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = (CMAP_MAP *)lc;
  if(!map -> internal.ghost) cmap_map_apply(map, nested_apply, list, proc_ctx);

  cmap_lifecycle_nested(lc, list, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_set(CMAP_MAP * map, const char * key, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP_ENTRY ** entry_stree = &map -> internal.entry_stree;
  CMAP_MAP_ENTRY * entry =
    (CMAP_MAP_ENTRY *)CMAP_STREE_FINDFN(entry, *entry_stree, key);
  if(entry == NULL)
  {
    entry = CMAP_MEM_ALLOC(CMAP_MAP_ENTRY);
    entry -> key = cmap_util_strdup(key);
    entry -> val = NULL;

    CMAP_STREE_ADDFN(entry, entry_stree, entry, key);
  }

  if(!map -> internal.ghost && (entry -> val != NULL))
    CMAP_DEC_REFS(entry -> val, proc_ctx);
  entry -> val = val;
  if(!map -> internal.ghost && (val != NULL)) CMAP_INC_REFS(val);

  return val;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_get(CMAP_MAP * map, const char * key)
{
  CMAP_MAP * ret = NULL;

  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)CMAP_STREE_FINDFN(
    entry, map -> internal.entry_stree, key);
  if(entry != NULL) ret = entry -> val;
  else
  {
    CMAP_MAP * prototype = map -> internal.prototype;
    if(prototype != NULL) ret = CMAP_GET(prototype, key);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_map_new(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  initargs.nature = CMAP_MAP_NATURE;
  initargs.prototype = map;
  initargs.proc_ctx = proc_ctx;
  return cmap_map_init(CMAP_MEM_ALLOC(CMAP_MAP), &initargs);
}

/*******************************************************************************
*******************************************************************************/

char cmap_map_has(CMAP_MAP * map, const char * key)
{
  return (CMAP_STREE_FINDFN(entry, map -> internal.entry_stree, key) != NULL);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST * keys;
  CMAP_PROC_CTX * proc_ctx;
} KEYS_APPLY_DATA;

static void keys_apply(const char * key, CMAP_MAP ** val, void * data,
  CMAP_PROC_CTX * proc_ctx)
{
  KEYS_APPLY_DATA * data_ = data;
  CMAP_LIST_PUSH(data_ -> keys, CMAP_STRING(key, 0, data_ -> proc_ctx));
}

CMAP_LIST * cmap_map_keys(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx)
{
  KEYS_APPLY_DATA data;
  data.keys = CMAP_LIST(0, proc_ctx);
  data.proc_ctx = proc_ctx;

  cmap_map_apply(map, keys_apply, &data, proc_ctx);

  return data.keys;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_MAP_ENTRY_FN fn;
  void * data;
  char ghost;
  CMAP_PROC_CTX * proc_ctx;
} APPLY_APPLY_DATA;

static void apply_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)node;
  APPLY_APPLY_DATA * data_ = data;

  CMAP_MAP * prev_val = entry -> val;
  data_ -> fn(entry -> key, &entry -> val, data_ -> data, data_ -> proc_ctx);
  if(!data_ -> ghost && (prev_val != entry -> val))
  {
    if(prev_val != NULL) CMAP_DEC_REFS(prev_val, data_ -> proc_ctx);
    if(entry -> val != NULL) CMAP_INC_REFS(entry -> val);
  }
}

void cmap_map_apply(CMAP_MAP * map, CMAP_MAP_ENTRY_FN fn, void * data,
  CMAP_PROC_CTX * proc_ctx)
{
  APPLY_APPLY_DATA data_ = {fn, data, map -> internal.ghost, proc_ctx};
  CMAP_STREE_QUICKAPPLYFN(map -> internal.entry_stree, apply_apply, &data_);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char ghost;
  CMAP_PROC_CTX * proc_ctx;
} CLEAN_APPLY_DATA;

static void clean_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)node;

  cmap_mem_free(entry -> key);

  CMAP_MAP * val = entry -> val;
  CLEAN_APPLY_DATA * data_ = data;
  if(!data_ -> ghost && (val != NULL)) CMAP_DEC_REFS(val, data_ -> proc_ctx);

  cmap_mem_free(entry);
}

void cmap_map_clean(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx)
{
  CLEAN_APPLY_DATA data = {map -> internal.ghost, proc_ctx};
  CMAP_STREE_QUICKAPPLYFN(map -> internal.entry_stree, clean_apply, &data);
  map -> internal.entry_stree = NULL;
}

/*******************************************************************************
*******************************************************************************/

void cmap_map_ghost(CMAP_MAP * map)
{
  map -> internal.ghost = CMAP_T;
}

char cmap_map_is_ghost(CMAP_MAP * map)
{
  return map -> internal.ghost;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;
  char ghost;
  CMAP_PROC_CTX * proc_ctx;
} DELETE_APPLY_DATA;

static void delete_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  CMAP_MAP_ENTRY * entry = (CMAP_MAP_ENTRY *)node;
  DELETE_APPLY_DATA * data_ = data;

  cmap_mem_free(entry -> key);

  CMAP_MAP * val = entry -> val;
  if(!data_ -> ghost && (val != NULL))
  {
    CMAP_LIFECYCLE * lc = data_ -> lc;
    cmap_log_debug("[%p][%s]-nested->[[%p]==>refs--]",
      lc, CMAP_NATURE_CHAR(lc), val);
    CMAP_DEC_REFS(val, data_ -> proc_ctx);
  }

  cmap_mem_free(entry);
}

void cmap_map_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP_INTERNAL * internal = &((CMAP_MAP *)lc) -> internal;

  DELETE_APPLY_DATA data = {lc, internal -> ghost, proc_ctx};
  CMAP_STREE_QUICKAPPLYFN(internal -> entry_stree, delete_apply, &data);

  cmap_lifecycle_delete(lc, proc_ctx);
}

CMAP_MAP * cmap_map_init(CMAP_MAP * map, CMAP_INITARGS * initargs)
{
  cmap_lifecycle_init((CMAP_LIFECYCLE *)map, initargs);

  map -> internal.entry_stree = NULL;
  map -> internal.prototype = initargs -> prototype;
  map -> internal.ghost = CMAP_F;

  return map;
}

CMAP_MAP * cmap_map_create_root(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_map_new(NULL, proc_ctx);
}

CMAP_MAP * cmap_map_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  return cmap_map_new(cmap_prototypestore_map(ps, proc_ctx), proc_ctx);
}
