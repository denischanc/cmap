
#include "cmap-pool.h"

#include "cmap-fw.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_POOL_HANDLER * handler_;
  int size_;
  CMAP_LIST * list_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void pool__delete(CMAP_POOL * this)
{
  CMAP_MEM * mem = cmap_kernel() -> mem_;

  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  CMAP_FREE(internal, mem);

  CMAP_FREE(this, mem);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * pool__take(CMAP_POOL * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  CMAP_LIST * list = internal -> list_;

  if(CMAP_CALL(list, size) <= 0)
  {
    return internal -> handler_ -> create();
  }
  else
  {
    return CMAP_CALL(list, pop);
  }
}

/*******************************************************************************
*******************************************************************************/

static void pool__release(CMAP_POOL * this, CMAP_MAP * map)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  CMAP_LIST * list = internal -> list_;

  if(CMAP_CALL(list, size) < internal -> size_)
  {
    internal -> handler_ -> clean(map);
    CMAP_PUSH(list, map);
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_POOL * cmap_pool_create(CMAP_POOL_HANDLER * handler, int size)
{
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_ALLOC_PTR(pool, CMAP_POOL, mem);

  CMAP_ALLOC_PTR(internal, CMAP_INTERNAL, mem);
  internal -> handler_ = handler;
  internal -> size_ = size;
  internal -> list_ = CMAP_LIST(size, CMAP_AISLE_KERNEL);

  pool -> internal_ = internal;
  pool -> delete = pool__delete;
  pool -> take = pool__take;
  pool -> release = pool__release;

  return pool;
}
