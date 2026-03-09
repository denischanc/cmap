
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
  int size; \
 \
  CMAP_LIST * availables; \
}; \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
type cmap_pool_##name##_take(CMAP_POOL_##NAME * pool, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_LIST * availables = pool -> availables; \
  if(cmap_list_size(availables) > 0) \
    return (type)CMAP_LIST_POP(availables, proc_ctx); \
  else return cmap_pool_handler_##name##_create(proc_ctx); \
} \
 \
void cmap_pool_##name##_release(CMAP_POOL_##NAME * pool, type e, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_LIST * availables = pool -> availables; \
  if(cmap_list_size(availables) < pool -> size) \
  { \
    cmap_pool_handler_##name##_clean(e, proc_ctx); \
    CMAP_LIST_PUSH(availables, e); \
  } \
} \
 \
/****************************************************************************/ \
/****************************************************************************/ \
 \
void cmap_pool_##name##_delete(CMAP_POOL_##NAME * pool, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_DEC_REFS(pool -> availables, proc_ctx); \
 \
  cmap_mem_free(pool); \
} \
 \
CMAP_POOL_##NAME * cmap_pool_##name##_create(int size, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if(size <= 0) size = cmap_config_pool_size(); \
 \
  CMAP_MEM_ALLOC_PTR(pool, CMAP_POOL_##NAME); \
  pool -> size = size; \
  pool -> availables = CMAP_LIST(size, proc_ctx); \
  CMAP_INC_REFS(pool -> availables); \
  return pool; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_LOOP(IMPL)
