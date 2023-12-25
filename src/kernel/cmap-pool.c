
#include "cmap-pool.h"

#include "cmap.h"
#include "cmap-pool-handlers.h"
#include "cmap-mem.h"
#include "cmap-kernel.h"
#include "cmap-list.h"

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
static const char * type##_nature(CMAP_LIFECYCLE * this) \
{ \
  return #type "_pool"; \
} \
 \
static void type##_delete(CMAP_LIFECYCLE * this) \
{ \
  INTERNAL * internal = (INTERNAL *)(((CMAP_POOL_##TYPE *)this) + 1); \
  CMAP_DEC_REFS(internal -> list); \
 \
  cmap_lifecycle_public.delete(this); \
} \
 \
static CMAP_##TYPE * type##_take(CMAP_POOL_##TYPE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  CMAP_LIST * list = internal -> list; \
 \
  if(CMAP_CALL(list, size) > 0) return (CMAP_##TYPE *)CMAP_CALL(list, pop); \
  else return cmap_pool_handler_##type##_public.create(proc_ctx); \
} \
 \
static void type##_release(CMAP_POOL_##TYPE * this, CMAP_##TYPE * e) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  CMAP_LIST * list = internal -> list; \
 \
  if(CMAP_CALL(list, size) < internal -> size) \
  { \
    cmap_pool_handler_##type##_public.clean(e); \
    CMAP_LIST_PUSH(list, e); \
  } \
} \
 \
static CMAP_POOL_##TYPE * type##_create(int size, CMAP_PROC_CTX * proc_ctx) \
{ \
  CMAP_MEM * mem = CMAP_KERNEL_MEM; \
  CMAP_POOL_##TYPE * this = (CMAP_POOL_##TYPE *)mem -> alloc( \
    sizeof(CMAP_POOL_##TYPE) + sizeof(INTERNAL)); \
 \
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this; \
  cmap_lifecycle_public.init(lc, proc_ctx); \
  lc -> delete = type##_delete; \
  lc -> nature = type##_nature; \
 \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  internal -> size = size; \
  internal -> list = CMAP_LIST(size, proc_ctx); \
  CMAP_INC_REFS(internal -> list); \
 \
  this -> take = type##_take; \
  this -> release = type##_release; \
 \
  return this; \
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
