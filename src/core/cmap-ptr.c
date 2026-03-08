
#include "cmap-ptr.h"

#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_PTR_NATURE = "ptr";

/*******************************************************************************
*******************************************************************************/

void * cmap_ptr_get(CMAP_PTR * ptr)
{
  return ptr -> internal.ptr;
}

/*******************************************************************************
*******************************************************************************/

void ** cmap_ptr_ref(CMAP_PTR * ptr)
{
  return &ptr -> internal.ptr;
}

/*******************************************************************************
*******************************************************************************/

void cmap_ptr_delete(CMAP_LIFECYCLE * lc)
{
  CMAP_PTR * ptr = (CMAP_PTR *)lc;

  void * ptr_ = ptr -> internal.ptr;
  if(ptr_ != NULL)
  {
    if(ptr -> internal.delete_ptr != NULL) ptr -> internal.delete_ptr(ptr_);
    if(ptr -> internal.allocated_by_this) cmap_mem_free(ptr_);
  }

  cmap_map_delete(lc);
}

CMAP_PTR * cmap_ptr_init(CMAP_PTR * ptr, CMAP_INITARGS * initargs, int size,
  CMAP_PTR_DELETE delete_ptr)
{
  cmap_map_init((CMAP_MAP *)ptr, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)ptr;
  lc -> delete = cmap_ptr_delete;

  if(size == 0)
  {
    ptr -> internal.allocated_by_this = CMAP_F;
    ptr -> internal.ptr = NULL;
  }
  else
  {
    ptr -> internal.allocated_by_this = CMAP_T;
    ptr -> internal.ptr = cmap_mem_alloc(size);
  }
  ptr -> internal.delete_ptr = delete_ptr;

  return ptr;
}

CMAP_PTR * cmap_ptr_create(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_PTR_NATURE;
  initargs.prototype = cmap_prototypestore_ptr(ps, proc_ctx);
  initargs.proc_ctx = proc_ctx;

  return cmap_ptr_init(CMAP_MEM_ALLOC(CMAP_PTR), &initargs, size, delete_ptr);
}
