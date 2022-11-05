
#include "cmap-pool.h"

#include <stdlib.h>
#include "cmap-pool-handlers.h"
#include "cmap-mem.h"
#include "cmap-kernel.h"
#include "cmap-common.h"

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
static void type##_delete(CMAP_POOL_##TYPE * this) \
{ \
  CMAP_MEM * mem = cmap_kernel_public.mem(); \
 \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  CMAP_LIST * list = internal -> list; \
 \
  CMAP_##TYPE * e; \
  while((e = (CMAP_##TYPE *)CMAP_CALL(list, pop)) != NULL) \
  { \
    cmap_pool_handler_##type##_public.delete(e); \
  } \
 \
  CMAP_DELETE(list); \
  CMAP_MEM_FREE(internal, mem); \
 \
  CMAP_MEM_FREE(this, mem); \
} \
 \
static CMAP_##TYPE * type##_take(CMAP_POOL_##TYPE * this) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  CMAP_LIST * list = internal -> list; \
 \
  if(CMAP_CALL(list, size) <= 0) \
  { \
    return cmap_pool_handler_##type##_public.create(); \
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
CMAP_POOL_##TYPE * type##_create(int size) \
{ \
  CMAP_MEM * mem = cmap_kernel_public.mem(); \
  CMAP_MEM_ALLOC_PTR(pool, CMAP_POOL_##TYPE, mem); \
 \
  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem); \
  internal -> size = size; \
  internal -> list = CMAP_LIST(size, NULL); \
 \
  pool -> internal = internal; \
  pool -> delete = type##_delete; \
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
