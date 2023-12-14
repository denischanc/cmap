
#include "cmap-lifecycle.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-list.h"
#include "cmap-env.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int nb_ref;

  CMAP_ENV * env;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void inc_ref(CMAP_LIFECYCLE * this)
{
  ((INTERNAL *)this -> internal) -> nb_ref++;
}

/*******************************************************************************
*******************************************************************************/

static void dec_ref(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> nb_ref--;

  if(internal -> nb_ref == 0)
  {
    CMAP_PROC_CTX * proc_ctx = CMAP_CALL(internal -> env, proc_ctx);
    CMAP_CALL_ARGS(proc_ctx, local_stack_add, this);
  }
}

/*******************************************************************************
*******************************************************************************/

static char dec_ref_or_deep_delete_last_ref(CMAP_LIFECYCLE * this)
{
  if(((INTERNAL *)this -> internal) -> nb_ref != 1)
  {
    CMAP_DEC_REF(this);
    return CMAP_F;
  }
  else
  {
    CMAP_CALL(this, deep_delete);
    return CMAP_T;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * lc)
{
  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_FREE(lc -> internal, mem);
  CMAP_MEM_FREE(lc, mem);
}

static void deep_delete(CMAP_LIFECYCLE * lc)
{
  CMAP_CALL(lc, delete);
}

static void init(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> nb_ref = 0;
  internal -> env = CMAP_CALL(proc_ctx, env);

  lc -> internal = internal;
  lc -> delete = delete;
  lc -> deep_delete = deep_delete;
  lc -> inc_ref = inc_ref;
  lc -> dec_ref = dec_ref;
  lc -> dec_ref_or_deep_delete_last_ref = dec_ref_or_deep_delete_last_ref;

  CMAP_CALL_ARGS(proc_ctx, local_stack_add, lc);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public =
{
  init,
  delete,
  deep_delete,
  inc_ref,
  dec_ref,
  dec_ref_or_deep_delete_last_ref
};
