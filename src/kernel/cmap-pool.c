
#include "cmap-pool.h"

#include "cmap-pool-handler.h"
#include "cmap-mem.h"
#include "cmap-kernel.h"
#include "cmap-list.h"
#include "cmap-sset.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size;

  CMAP_LIST * availables;

  CMAP_SSET_LC * all;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define IMPL(TYPE, type) \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void type##_nested_apply(CMAP_LIFECYCLE ** e, void * data) \
{ \
  CMAP_CALL_ARGS((CMAP_SLIST_LC_PTR *)data, push, e); \
} \
 \
static void type##_nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list) \
{ \
  INTERNAL * internal = (INTERNAL *)(((CMAP_POOL_##TYPE *)this) + 1); \
  CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)&internal -> availables); \
  cmap_sset_lc_public.apply(internal -> all, type##_nested_apply, list); \
 \
  cmap_lifecycle_public.nested(this, list); \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static CMAP_##TYPE * type##_take(CMAP_POOL_##TYPE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  CMAP_LIST * availables = internal -> availables; \
  if(CMAP_CALL(availables, size) > 0) \
    return (CMAP_##TYPE *)CMAP_LIST_POP(availables); \
  else \
  { \
    CMAP_##TYPE * e = cmap_pool_handler_##type##_public.create(proc_ctx); \
    cmap_sset_lc_public.add(&internal -> all, (CMAP_LIFECYCLE *)e); \
    CMAP_INC_REFS(e); \
    return e; \
  } \
} \
 \
static void type##_release(CMAP_POOL_##TYPE * this, CMAP_##TYPE * e) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
 \
  CMAP_LIST * availables = internal -> availables; \
  if(cmap_sset_lc_public.size(internal -> all) < internal -> size) \
  { \
    cmap_pool_handler_##type##_public.clean(e); \
    CMAP_LIST_PUSH(availables, e); \
  } \
  else if(cmap_sset_lc_public.rm_v(&internal -> all, (CMAP_LIFECYCLE *)e)) \
    CMAP_DEC_REFS(e); \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void type##_delete(CMAP_LIFECYCLE * this) \
{ \
  INTERNAL * internal = (INTERNAL *)(((CMAP_POOL_##TYPE *)this) + 1); \
 \
  CMAP_DEC_REFS(internal -> availables); \
  while(internal -> all != NULL) \
  { \
    CMAP_LIFECYCLE * e = cmap_sset_lc_public.rm(&internal -> all); \
    CMAP_DEC_REFS(e); \
  } \
 \
  cmap_lifecycle_public.delete(this); \
} \
 \
static CMAP_POOL_##TYPE * type##_create(int size, CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE; \
  size = (size <= 0) ? kernel -> cfg() -> pool.size : size; \
 \
  CMAP_MEM * mem = kernel -> mem(); \
  CMAP_POOL_##TYPE * this = (CMAP_POOL_##TYPE *)mem -> alloc( \
    sizeof(CMAP_POOL_##TYPE) + sizeof(INTERNAL)); \
 \
  CMAP_INITARGS initargs; \
  initargs.nature = #type "_pool"; \
  initargs.allocator = NULL; \
  initargs.proc_ctx = proc_ctx; \
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this; \
  cmap_lifecycle_public.init(lc, &initargs); \
  lc -> delete = type##_delete; \
  lc -> nested = type##_nested; \
 \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  internal -> size = size; \
  internal -> availables = CMAP_LIST(size, proc_ctx); \
  CMAP_INC_REFS(internal -> availables); \
  internal -> all = NULL; \
 \
  this -> take = type##_take; \
  this -> release = type##_release; \
 \
  return this; \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
const CMAP_POOL_##TYPE##_PUBLIC cmap_pool_##type##_public = { type##_create };

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_LOOP(IMPL)
