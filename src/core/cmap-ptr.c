
#include "cmap-ptr.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-prototype-ptr.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  void * ptr;
  char do_free;

  CMAP_PTR_DELETE delete_ptr;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_PTR_NATURE = "ptr";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_PTR_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void * get(CMAP_PTR * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> ptr;
}

/*******************************************************************************
*******************************************************************************/

static void ** ref(CMAP_PTR * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return &internal -> ptr;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_PTR * ptr)
{
  INTERNAL * internal = (INTERNAL *)ptr -> internal;

  if(internal -> delete_ptr != NULL)
    internal -> delete_ptr(internal -> ptr);

  if(internal -> do_free) CMAP_KERNEL_FREE(internal -> ptr);

  CMAP_KERNEL_FREE(internal);

  return cmap_map_public.delete((CMAP_MAP *)ptr);
}

static CMAP_MAP * delete_(CMAP_MAP * ptr)
{
  return delete((CMAP_PTR *)ptr);
}

static void init(CMAP_PTR * ptr, int size, CMAP_PTR_DELETE delete_ptr)
{
  CMAP_MAP * super = &ptr -> super;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  if(size == 0)
  {
    internal -> ptr = NULL;
    internal -> do_free = CMAP_F;
  }
  else
  {
    internal -> ptr = cmap_kernel_public.mem() -> alloc(size);
    internal -> do_free = CMAP_T;
  }
  internal -> delete_ptr = delete_ptr;

  ptr -> internal = internal;
  ptr -> get = get;
  ptr -> ref = ref;
}

static CMAP_PTR * create(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * prototype_ptr = cmap_prototype_ptr_public.instance(proc_ctx);
  CMAP_PTR * ptr = (CMAP_PTR *)CMAP_CALL_ARGS(prototype_ptr, new,
    sizeof(CMAP_PTR), proc_ctx, aisle);
  init(ptr, size, delete_ptr);
  return ptr;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PTR_PUBLIC cmap_ptr_public =
{
  create,
  init,
  delete,
  get,
  ref
};
