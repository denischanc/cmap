
#include "cmap-lifecycle.h"

#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int nb_refs;

  CMAP_ENV * env;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_LIFECYCLE * this)
{
  return "lifecycle";
}

/*******************************************************************************
*******************************************************************************/

static void inc_refs(CMAP_LIFECYCLE * this)
{
  ((INTERNAL *)this -> internal) -> nb_refs++;
}

/*******************************************************************************
*******************************************************************************/

static int nb_refs(CMAP_LIFECYCLE * this)
{
  return ((INTERNAL *)this -> internal) -> nb_refs;
}

/*******************************************************************************
*******************************************************************************/

static void dec_refs(CMAP_LIFECYCLE * this)
{
  CMAP_CALL(this, dec_refs_only);

  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_PROC_CTX * proc_ctx = CMAP_CALL(internal -> env, proc_ctx);
  CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_F);
}

/*******************************************************************************
*******************************************************************************/

static void dec_refs_only(CMAP_LIFECYCLE * this)
{
  ((INTERNAL *)this -> internal) -> nb_refs--;
}

/*******************************************************************************
*******************************************************************************/

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_MEM_FREE(this -> internal, mem);
  CMAP_MEM_FREE(this, mem);
}

static void init(CMAP_LIFECYCLE * this, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> nb_refs = 0;
  internal -> env = CMAP_CALL(proc_ctx, env);

  this -> internal = internal;
  this -> delete = delete;
  this -> nature = nature;
  this -> inc_refs = inc_refs;
  this -> nb_refs = nb_refs;
  this -> dec_refs = dec_refs;
  this -> dec_refs_only = dec_refs_only;
  this -> nested = nested;

  CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_T);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public =
{
  init, delete,
  inc_refs, nb_refs, dec_refs, dec_refs_only,
  nested
};
