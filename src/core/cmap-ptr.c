
#include "cmap-ptr.h"

#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"

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

static const char * nature(CMAP_LIFECYCLE * this)
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

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  INTERNAL * internal = (INTERNAL *)((CMAP_PTR *)this) -> internal;

  if(internal -> delete_ptr != NULL)
    internal -> delete_ptr(internal -> ptr);

  if(internal -> do_free) CMAP_KERNEL_FREE(internal -> ptr);

  CMAP_KERNEL_FREE(internal);

  cmap_map_public.delete(this);
}

static void init(CMAP_PTR * this, int size, CMAP_PTR_DELETE delete_ptr)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nature = nature;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  if(size == 0)
  {
    internal -> ptr = NULL;
    internal -> do_free = CMAP_F;
  }
  else
  {
    internal -> ptr = CMAP_KERNEL_MEM -> alloc(size);
    internal -> do_free = CMAP_T;
  }
  internal -> delete_ptr = delete_ptr;

  this -> internal = internal;
  this -> get = get;
  this -> ref = ref;
}

static CMAP_PTR * create(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_ptr = CMAP_CALL_ARGS(ps, ptr_, proc_ctx);
  CMAP_PTR * this = CMAP_PROTOTYPE_NEW(prototype_ptr, CMAP_PTR, proc_ctx);
  init(this, size, delete_ptr);
  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PTR_PUBLIC cmap_ptr_public =
{
  create, init, delete,
  get,
  ref
};
