
#include "cmap-pool.h"

#include "cmap-pool-handlers.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size_;
  CMAP_LIST * list_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define CMAP_POOL_IMPL(name, lower_name, _struct, name_s, handler) \
 \
static void pool_##lower_name##__delete(CMAP_POOL_##name * this) \
{ \
  CMAP_MEM * mem = cmap_kernel() -> mem_; \
 \
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_; \
  CMAP_FREE(internal, mem); \
 \
  CMAP_FREE(this, mem); \
} \
 \
static _struct * pool_##lower_name##__take(CMAP_POOL_##name * this) \
{ \
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_; \
  CMAP_LIST * list_i = internal -> list_; \
 \
  if(CMAP_CALL(list_i, size) <= 0) \
  { \
    return handler -> create(); \
  } \
  else \
  { \
    return (_struct *)CMAP_CALL(list, pop); \
  } \
} \
 \
static void pool_##lower_name##__release(CMAP_POOL_##name * this, \
  _struct * name_s) \
{ \
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_; \
  CMAP_LIST * list_i = internal -> list_; \
 \
  if(CMAP_CALL(list_i, size) < internal -> size_) \
  { \
    handler -> clean(name_s); \
    CMAP_PUSH(list, map); \
  } \
  else \
  { \
    handler -> delete(name_s); \
  } \
} \
 \
CMAP_POOL_##name * cmap_pool_##lower_name##_create(int size) \
{ \
  CMAP_MEM * mem = cmap_kernel() -> mem_; \
  CMAP_ALLOC_PTR(pool, CMAP_POOL_##name, mem); \
 \
  CMAP_ALLOC_PTR(internal, CMAP_INTERNAL, mem); \
  internal -> size_ = size; \
  internal -> list_ = CMAP_LIST(size, CMAP_AISLE_KERNEL); \
 \
  pool -> internal_ = internal; \
  pool -> delete = pool__delete; \
  pool -> take = pool__take; \
  pool -> release = pool__release; \
  pool -> handler = pool__handler; \
 \
  return pool; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_IMPL(LIST, list, CMAP_LIST, list)
