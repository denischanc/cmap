
#include "cmap-lifecycle.h"

#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

static const char * REF_STATE_FREE = "free";
static const char * REF_STATE_STORED = "stored";
static const char * REF_STATE_HOOKED = "hooked";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  const char * nature;

  int nb_refs;

  CMAP_ENV * env;

  CMAP_LIFECYCLE * allocator;

  CMAP_REFSWATCHER * refswatcher;
  const char * ref_state;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_LIFECYCLE * this)
{
  return ((INTERNAL *)(this -> internal)) -> nature;
}

/*******************************************************************************
*******************************************************************************/

static void inc_refs(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;

  if(internal -> refswatcher != NULL)
  {
    CMAP_CALL_ARGS(internal -> refswatcher, rm_only, this);
    internal -> refswatcher = NULL;

    cmap_log_public.debug("[%p][%s] not watched", this, internal -> nature);
  }
  else internal -> nb_refs++;

  if(internal -> ref_state == REF_STATE_STORED)
  {
    internal -> ref_state = REF_STATE_HOOKED;

    cmap_log_public.debug("[%p][%s] ref_state = [%s]", this,
      internal -> nature, internal -> ref_state);
  }
}

static void inc_refs_only(CMAP_LIFECYCLE * this)
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
  INTERNAL * internal = this -> internal;
  if(internal -> ref_state == REF_STATE_FREE)
  {
    CMAP_PROC_CTX * proc_ctx = CMAP_CALL(internal -> env, proc_ctx);
    CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_F);
    internal -> ref_state = REF_STATE_STORED;

    cmap_log_public.debug("[%p][%s] ref_state = [%s]", this,
      internal -> nature, internal -> ref_state);
  }
  else
  {
    internal -> nb_refs--;

    if(internal -> ref_state == REF_STATE_HOOKED)
    {
      internal -> ref_state = REF_STATE_STORED;

      cmap_log_public.debug("[%p][%s] ref_state = [%s]", this,
        internal -> nature, internal -> ref_state);
    }
  }
}

static void dec_refs_only(CMAP_LIFECYCLE * this)
{
  ((INTERNAL *)this -> internal) -> nb_refs--;
}

static void dec_refs_only_nb(CMAP_LIFECYCLE * this, int nb)
{
  ((INTERNAL *)this -> internal) -> nb_refs -= nb;
}

/*******************************************************************************
*******************************************************************************/

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
}

/*******************************************************************************
*******************************************************************************/

static void allocated_deleted(CMAP_LIFECYCLE * this, CMAP_LIFECYCLE * lc)
{
}

/*******************************************************************************
*******************************************************************************/

static void watched(CMAP_LIFECYCLE * this, CMAP_REFSWATCHER * refswatcher)
{
  INTERNAL * internal = this -> internal;
  internal -> refswatcher = refswatcher;

  cmap_log_public.debug("[%p][%s] %s watched", this, internal -> nature,
    (refswatcher != NULL) ? "is" : "not");
}

static CMAP_REFSWATCHER * is_watched(CMAP_LIFECYCLE * this)
{
  return ((INTERNAL *)this -> internal) -> refswatcher;
}

/*******************************************************************************
*******************************************************************************/

static void stored(CMAP_LIFECYCLE * this, char ok)
{
  INTERNAL * internal = this -> internal;
  internal -> ref_state = ok ? REF_STATE_STORED : REF_STATE_FREE;

  cmap_log_public.debug("[%p][%s] ref_state = [%s]", this, internal -> nature,
    internal -> ref_state);
}

static char is_stored(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;
  return ((internal -> ref_state == REF_STATE_STORED) ||
    (internal -> ref_state == REF_STATE_HOOKED));
}

static char in_refs(CMAP_LIFECYCLE * this)
{
  return (((INTERNAL *)this -> internal) -> ref_state == REF_STATE_STORED);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_LIFECYCLE * allocator = internal -> allocator;

  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_MEM_FREE(internal, mem);
  if(allocator == NULL) CMAP_MEM_FREE(this, mem);
  else CMAP_CALL_ARGS(allocator, allocated_deleted, this);
}

static CMAP_LIFECYCLE * init(CMAP_LIFECYCLE * this, CMAP_INITARGS * initargs)
{
  CMAP_PROC_CTX * proc_ctx = initargs -> proc_ctx;
  CMAP_LIFECYCLE * allocator = initargs -> allocator;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> nature = initargs -> nature;
  internal -> nb_refs = 1;
  internal -> env = CMAP_CALL(proc_ctx, env);
  internal -> allocator = allocator;
  internal -> refswatcher = NULL;
  internal -> ref_state = REF_STATE_STORED;

  CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_T);

  this -> internal = internal;
  this -> delete = delete;
  this -> nature = nature;
  this -> inc_refs = inc_refs;
  this -> inc_refs_only = inc_refs_only;
  this -> nb_refs = nb_refs;
  this -> dec_refs = dec_refs;
  this -> dec_refs_only = dec_refs_only;
  this -> dec_refs_only_nb = dec_refs_only_nb;
  this -> nested = nested;
  this -> allocated_deleted = allocated_deleted;
  this -> watched = watched;
  this -> is_watched = is_watched;
  this -> stored = stored;
  this -> is_stored = is_stored;
  this -> in_refs = in_refs;

  cmap_log_public.debug("[%p][%s] creation", this, internal -> nature);

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public =
{
  init, delete,
  inc_refs, inc_refs_only, nb_refs, dec_refs, dec_refs_only, dec_refs_only_nb,
  nested,
  allocated_deleted,
  watched, is_watched,
  stored, is_stored, in_refs
};
