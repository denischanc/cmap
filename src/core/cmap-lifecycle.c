
#include "cmap-lifecycle.h"

#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"
#include "cmap-log.h"
#include "cmap-util.h"
#include "cmap-config.h"

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

  uint64_t watch_time_us;
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

static inline uint64_t next_watch_time_us()
{
  return cmap_util_public.time_us() + cmap_config_refs_check_zombie_time_us();
}

/*******************************************************************************
*******************************************************************************/

static inline void set_ref_state(CMAP_LIFECYCLE * this, INTERNAL * internal,
  const char * ref_state)
{
  internal -> ref_state = ref_state;

  cmap_log_public.debug("[%p][%s] ref_state = [%s], nb_refs = [%d]", this,
    internal -> nature, ref_state, internal -> nb_refs);
}

/*******************************************************************************
*******************************************************************************/

static inline void do_store(CMAP_LIFECYCLE * this, INTERNAL * internal)
{
  CMAP_PROC_CTX * proc_ctx = CMAP_CALL(internal -> env, proc_ctx);
  CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_F);
  set_ref_state(this, internal, REF_STATE_STORED);
}

/*******************************************************************************
*******************************************************************************/

static void inc_refs(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;

  internal -> nb_refs++;

  if(internal -> watch_time_us > 0)
    internal -> watch_time_us = next_watch_time_us();

  if(internal -> ref_state == REF_STATE_STORED)
    set_ref_state(this, internal, REF_STATE_HOOKED);
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

  internal -> nb_refs--;

  if(internal -> ref_state == REF_STATE_FREE)
    do_store(this, internal);
  else if(internal -> ref_state == REF_STATE_HOOKED)
    set_ref_state(this, internal, REF_STATE_STORED);
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

static void watched(CMAP_LIFECYCLE * this, char val)
{
  INTERNAL * internal = this -> internal;

  if(val)
  {
    if(internal -> watch_time_us <= 0)
      cmap_log_public.debug("[%p][%s] is watched", this, internal -> nature);

    internal -> watch_time_us = next_watch_time_us();
  }
  else
  {
    cmap_log_public.debug("[%p][%s] not watched", this, internal -> nature);

    internal -> watch_time_us = 0;
  }
}

static char is_watched(CMAP_LIFECYCLE * this)
{
  return (((INTERNAL *)this -> internal) -> watch_time_us > 0);
}

static uint64_t watch_time_us(CMAP_LIFECYCLE * this)
{
  return ((INTERNAL *)this -> internal) -> watch_time_us;
}

/*******************************************************************************
*******************************************************************************/

static void store(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;
  if(internal -> ref_state == REF_STATE_FREE) do_store(this, internal);
}

/*******************************************************************************
*******************************************************************************/

static char in_refs(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;
  const char * ref_state = internal -> ref_state;

  set_ref_state(this, internal, REF_STATE_FREE);

  return (ref_state != REF_STATE_HOOKED);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_from_refswatcher(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;
  if(internal -> watch_time_us > 0)
  {
    CMAP_REFSWATCHER * refswatcher = CMAP_CALL(internal -> env, refswatcher);
    CMAP_CALL_ARGS(refswatcher, rm, this);
  }
}

static inline void rm_from_local_refs(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = this -> internal;
  if(internal -> ref_state != REF_STATE_FREE)
  {
    CMAP_PROC_CTX * proc_ctx = CMAP_CALL(internal -> env, proc_ctx);
    CMAP_CALL_ARGS(proc_ctx, local_refs_rm, this);
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  rm_from_refswatcher(this);
  rm_from_local_refs(this);

  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_LIFECYCLE * allocator = internal -> allocator;

  CMAP_MEM_VAR;
  CMAP_MEM_FREE(internal, mem);
  if(allocator == NULL) CMAP_MEM_FREE(this, mem);
  else CMAP_CALL_ARGS(allocator, allocated_deleted, this);
}

static CMAP_LIFECYCLE * init(CMAP_LIFECYCLE * this, CMAP_INITARGS * initargs)
{
  CMAP_PROC_CTX * proc_ctx = initargs -> proc_ctx;
  CMAP_LIFECYCLE * allocator = initargs -> allocator;

  CMAP_MEM_INSTANCE_ALLOC_PTR(internal, INTERNAL);
  internal -> nature = initargs -> nature;
  internal -> nb_refs = 0;
  internal -> env = CMAP_CALL(proc_ctx, env);
  internal -> allocator = allocator;
  internal -> watch_time_us = 0;
  internal -> ref_state = REF_STATE_STORED;

  CMAP_CALL_ARGS(proc_ctx, local_refs_add, this, CMAP_T);

  this -> internal = internal;
  this -> delete = delete;
  this -> nature = nature;
  this -> inc_refs = inc_refs;
  this -> nb_refs = nb_refs;
  this -> dec_refs = dec_refs;
  this -> nested = nested;
  this -> allocated_deleted = allocated_deleted;
  this -> watched = watched;
  this -> is_watched = is_watched;
  this -> watch_time_us = watch_time_us;
  this -> store = store;
  this -> in_refs = in_refs;

  cmap_log_public.debug("[%p][%s] creation", this, internal -> nature);

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public =
{
  init, delete,
  inc_refs, nb_refs, dec_refs,
  nested,
  allocated_deleted,
  watched, is_watched, watch_time_us,
  store,
  in_refs
};
