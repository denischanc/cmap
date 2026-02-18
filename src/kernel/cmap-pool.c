
#include "cmap-pool.h"

#include "cmap-pool-handler.h"
#include "cmap-mem.h"
#include "cmap-list.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size;

  CMAP_LIST * availables;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define IMPL(NAME, name, type) \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void name##_nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list) \
{ \
  INTERNAL * internal = (INTERNAL *)(((CMAP_POOL_##NAME *)this) + 1); \
  CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)&internal -> availables); \
 \
  cmap_lifecycle_public.nested(this, list); \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static type name##_take(CMAP_POOL_##NAME * this, CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  CMAP_LIST * availables = internal -> availables; \
  if(CMAP_CALL(availables, size) > 0) \
    return (type)CMAP_LIST_POP(availables); \
  else return cmap_pool_handler_##name##_public.create(proc_ctx); \
} \
 \
static void name##_release(CMAP_POOL_##NAME * this, type e) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  CMAP_LIST * availables = internal -> availables; \
  if(CMAP_CALL(availables, size) < internal -> size) \
  { \
    cmap_pool_handler_##name##_public.clean(e); \
    CMAP_LIST_PUSH(availables, e); \
  } \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void name##_delete(CMAP_LIFECYCLE * this) \
{ \
  INTERNAL * internal = (INTERNAL *)(((CMAP_POOL_##NAME *)this) + 1); \
 \
  CMAP_DEC_REFS(internal -> availables); \
 \
  cmap_lifecycle_public.delete(this); \
} \
 \
static CMAP_POOL_##NAME * name##_create(int size, CMAP_PROC_CTX * proc_ctx) \
{ \
  if(size <= 0) size = cmap_config_pool_size(); \
 \
  CMAP_POOL_##NAME * this = cmap_mem_alloc( \
    sizeof(CMAP_POOL_##NAME) + sizeof(INTERNAL)); \
 \
  CMAP_INITARGS initargs; \
  initargs.nature = #name "_pool"; \
  initargs.allocator = NULL; \
  initargs.proc_ctx = proc_ctx; \
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this; \
  cmap_lifecycle_public.init(lc, &initargs); \
  lc -> delete = name##_delete; \
  lc -> nested = name##_nested; \
 \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  internal -> size = size; \
  internal -> availables = CMAP_LIST(size, proc_ctx); \
  CMAP_INC_REFS(internal -> availables); \
 \
  this -> take = name##_take; \
  this -> release = name##_release; \
 \
  return this; \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
const CMAP_POOL_##NAME##_PUBLIC cmap_pool_##name##_public = {name##_create};

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_LOOP(IMPL)
