
#include "cmap-pool.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-pool-handlers.h"
#include "cmap-mem.h"
#include "cmap-kernel.h"
#include "cmap-list.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size;
  CMAP_LIST * list;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define CMAP_POOL_IMPL(TYPE, type) \
static CMAP_LIFECYCLE * type##_delete(CMAP_LIFECYCLE * lc) \
{ \
  INTERNAL * internal = (INTERNAL *)((CMAP_POOL_##TYPE *)lc) -> internal; \
  CMAP_LIST * list = internal -> list; \
 \
  CMAP_##TYPE * e; \
  while((e = (CMAP_##TYPE *)CMAP_LIST_SHIFT(list)) != NULL) \
  { \
    cmap_pool_handler_##type##_public.delete(e); \
  } \
 \
  CMAP_DELETE(list); \
  CMAP_KERNEL_FREE(internal); \
 \
  return cmap_lifecycle_public.delete(lc); \
} \
 \
static CMAP_##TYPE * type##_take(CMAP_POOL_##TYPE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  CMAP_LIST * list = internal -> list; \
 \
  if(CMAP_CALL(list, size) <= 0) \
  { \
    return cmap_pool_handler_##type##_public.create(proc_ctx); \
  } \
  else \
  { \
    return (CMAP_##TYPE *)CMAP_CALL(list, pop); \
  } \
} \
 \
static void type##_release(CMAP_POOL_##TYPE * this, CMAP_##TYPE * e) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  CMAP_LIST * list = internal -> list; \
 \
  if(CMAP_CALL(list, size) < internal -> size) \
  { \
    cmap_pool_handler_##type##_public.clean(e); \
    CMAP_LIST_PUSH(list, e); \
  } \
  else \
  { \
    cmap_pool_handler_##type##_public.delete(e); \
  } \
} \
 \
static CMAP_POOL_##TYPE * type##_create(int size, CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_MEM * mem = cmap_kernel_public.mem(); \
  CMAP_MEM_ALLOC_PTR(pool, CMAP_POOL_##TYPE, mem); \
 \
  cmap_lifecycle_public.init(&pool -> super, proc_ctx, CMAP_AISLE_GLOBAL); \
  pool -> super.delete = type##_delete; \
 \
  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem); \
  internal -> size = size; \
  internal -> list = CMAP_LIST(size, proc_ctx, NULL); \
 \
  pool -> internal = internal; \
  pool -> take = type##_take; \
  pool -> release = type##_release; \
 \
  return pool; \
} \
 \
const CMAP_POOL_##TYPE##_PUBLIC cmap_pool_##type##_public = \
{ \
  type##_create \
};

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_IMPL(LIST, list)
CMAP_POOL_IMPL(STRING, string)
CMAP_POOL_IMPL(INT, int)
