
#include "cmap-pool.h"

#include <stdlib.h>
#include "cmap-pool-handlers.h"
#include "cmap-fw.h"

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
  CMAP_LIST * _list = internal -> list_; \
   \
  _struct * name_s; \
  while((name_s = (_struct *)CMAP_CALL(_list, pop)) != NULL) \
  { \
    handler -> delete(name_s); \
  } \
   \
  CMAP_DELETE(_list); \
  CMAP_FREE(internal, mem); \
 \
  CMAP_FREE(this, mem); \
} \
 \
static _struct * pool_##lower_name##__take(CMAP_POOL_##name * this) \
{ \
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_; \
  CMAP_LIST * _list = internal -> list_; \
 \
  if(CMAP_CALL(_list, size) <= 0) \
  { \
    return handler -> create(); \
  } \
  else \
  { \
    return (_struct *)CMAP_CALL(_list, pop); \
  } \
} \
 \
static void pool_##lower_name##__release(CMAP_POOL_##name * this, \
  _struct * name_s) \
{ \
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_; \
  CMAP_LIST * _list = internal -> list_; \
 \
  if(CMAP_CALL(_list, size) < internal -> size_) \
  { \
    handler -> clean(name_s); \
    CMAP_PUSH(_list, name_s); \
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
  internal -> list_ = CMAP_LIST(size, NULL); \
 \
  pool -> internal_ = internal; \
  pool -> delete = pool_##lower_name##__delete; \
  pool -> take = pool_##lower_name##__take; \
  pool -> release = pool_##lower_name##__release; \
 \
  return pool; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_POOL_IMPL(LIST, list, CMAP_LIST, list, cmap_pool_list_handler)
CMAP_POOL_IMPL(STRING, string, CMAP_STRING, string, cmap_pool_string_handler)
