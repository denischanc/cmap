
#include "cmap-lifecycle.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-aisle.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * next;

  char is_ref;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static char is_ref(CMAP_LIFECYCLE * this)
{
  return ((INTERNAL *)this -> internal) -> is_ref;
}

/*******************************************************************************
*******************************************************************************/

static void fill_aislestore(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  INTERNAL * internal = (INTERNAL *)lc -> internal;

  internal -> is_ref = CMAP_T;

  if(!strcmp(aisle, CMAP_AISLE_LOCAL))
  {
    CMAP_LIST * local_stack = CMAP_CALL(proc_ctx, local_stack);
    if(local_stack != NULL) CMAP_LIST_PUSH(local_stack, lc);
    else
    {
      cmap_log_public.fatal("Try to create local cmap outside fn !!!");
      CMAP_KERNEL_INSTANCE -> fatal();
    }
  }
  else
  {
    CMAP_MAP * as = (CMAP_MAP *)CMAP_CALL(proc_ctx, aislestore);
    internal -> next = &(CMAP_GET(as, aisle)) -> super;
    CMAP_SET(as, aisle, lc);
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIFECYCLE * delete(CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)lc -> internal;
  CMAP_LIFECYCLE * next = internal -> next;

  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_FREE(internal, mem);
  CMAP_MEM_FREE(lc, mem);

  return next;
}

static void init(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> next = NULL;
  internal -> is_ref = CMAP_F;

  lc -> internal = internal;
  lc -> delete = delete;
  lc -> is_ref = is_ref;

  if(aisle != NULL) fill_aislestore(lc, proc_ctx, aisle);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public =
{
  init,
  delete,
  is_ref
};
