
#include "cmap-pool.h"

#include "cmap-lifecycle.h"
#include "cmap-pool-handler.h"
#include "cmap-mem.h"
#include "cmap-list.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(NAME, name, type) \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
struct CMAP_POOL_##NAME \
{ \
  CMAP_LIFECYCLE super; \
 \
  int size; \
 \
  CMAP_LIST * availables; \
}; \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void name##_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list) \
{ \
  CMAP_POOL_##NAME * pool = (CMAP_POOL_##NAME *)lc; \
  CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)&pool -> availables); \
 \
  cmap_lifecycle_public.nested(lc, list); \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
type cmap_pool_##name##_take(CMAP_POOL_##NAME * pool, CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_LIST * availables = pool -> availables; \
  if(CMAP_CALL(availables, size) > 0) return (type)CMAP_LIST_POP(availables); \
  else return cmap_pool_handler_##name##_create(proc_ctx); \
} \
 \
void cmap_pool_##name##_release(CMAP_POOL_##NAME * pool, type e) \
{ \
  CMAP_LIST * availables = pool -> availables; \
  if(CMAP_CALL(availables, size) < pool -> size) \
  { \
    cmap_pool_handler_##name##_clean(e); \
    CMAP_LIST_PUSH(availables, e); \
  } \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
static void name##_delete(CMAP_LIFECYCLE * lc) \
{ \
  CMAP_POOL_##NAME * pool = (CMAP_POOL_##NAME *)lc; \
 \
  CMAP_DEC_REFS(pool -> availables); \
 \
  cmap_lifecycle_public.delete(lc); \
} \
 \
CMAP_POOL_##NAME * cmap_pool_##name##_create(int size, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if(size <= 0) size = cmap_config_pool_size(); \
 \
  CMAP_MEM_ALLOC_PTR(pool, CMAP_POOL_##NAME); \
 \
  CMAP_INITARGS initargs; \
  initargs.nature = #name "_pool"; \
  initargs.allocator = NULL; \
  initargs.proc_ctx = proc_ctx; \
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)pool; \
  cmap_lifecycle_public.init(lc, &initargs); \
  lc -> delete = name##_delete; \
  lc -> nested = name##_nested; \
 \
  pool -> size = size; \
  pool -> availables = CMAP_LIST(size, proc_ctx); \
  CMAP_INC_REFS(pool -> availables); \
 \
  return pool; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_LOOP(IMPL)
