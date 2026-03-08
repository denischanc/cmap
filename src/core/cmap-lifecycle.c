
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

const char * cmap_lifecycle_nature(CMAP_LIFECYCLE * lc)
{
  return lc -> internal.nature;
}

/*******************************************************************************
*******************************************************************************/

static inline uint64_t next_watch_time_us()
{
  return cmap_util_time_us() + cmap_config_refs_check_zombie_time_us();
}

/*******************************************************************************
*******************************************************************************/

static inline void set_ref_state(CMAP_LIFECYCLE * lc, const char * ref_state)
{
  lc -> internal.ref_state = ref_state;

  cmap_log_debug("[%p][%s] ref_state = [%s], nb_refs = [%d]", lc,
    lc -> internal.nature, ref_state, lc -> internal.nb_refs);
}

/*******************************************************************************
*******************************************************************************/

static inline void do_store(CMAP_LIFECYCLE * lc)
{
  CMAP_PROC_CTX * proc_ctx = cmap_env_proc_ctx(lc -> internal.env);
  cmap_proc_ctx_local_refs_add(proc_ctx, lc, CMAP_F);
  set_ref_state(lc, REF_STATE_STORED);
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_inc_refs(CMAP_LIFECYCLE * lc)
{
  lc -> internal.nb_refs++;

  if(lc -> internal.watch_time_us > 0)
    lc -> internal.watch_time_us = next_watch_time_us();

  if(lc -> internal.ref_state == REF_STATE_STORED)
    set_ref_state(lc, REF_STATE_HOOKED);
}

/*******************************************************************************
*******************************************************************************/

int cmap_lifecycle_nb_refs(CMAP_LIFECYCLE * lc)
{
  return lc -> internal.nb_refs;
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_dec_refs(CMAP_LIFECYCLE * lc)
{
  lc -> internal.nb_refs--;

  if(lc -> internal.ref_state == REF_STATE_FREE) do_store(lc);
  else if(lc -> internal.ref_state == REF_STATE_HOOKED)
    set_ref_state(lc, REF_STATE_STORED);
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_watched(CMAP_LIFECYCLE * lc, char val)
{
  if(val)
  {
    if(lc -> internal.watch_time_us <= 0)
      cmap_log_debug("[%p][%s] is watched", lc, lc -> internal.nature);

    lc -> internal.watch_time_us = next_watch_time_us();
  }
  else
  {
    cmap_log_debug("[%p][%s] not watched", lc, lc -> internal.nature);

    lc -> internal.watch_time_us = 0;
  }
}

char cmap_lifecycle_is_watched(CMAP_LIFECYCLE * lc)
{
  return (lc -> internal.watch_time_us > 0);
}

uint64_t cmap_lifecycle_watch_time_us(CMAP_LIFECYCLE * lc)
{
  return lc -> internal.watch_time_us;
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_store(CMAP_LIFECYCLE * lc)
{
  if(lc -> internal.ref_state == REF_STATE_FREE) do_store(lc);
}

/*******************************************************************************
*******************************************************************************/

char cmap_lifecycle_in_refs(CMAP_LIFECYCLE * lc)
{
  const char * ref_state = lc -> internal.ref_state;

  set_ref_state(lc, REF_STATE_FREE);

  return (ref_state != REF_STATE_HOOKED);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_from_refswatcher(CMAP_LIFECYCLE * lc)
{
  if(lc -> internal.watch_time_us > 0)
  {
    CMAP_REFSWATCHER * refswatcher = cmap_env_refswatcher(lc -> internal.env);
    cmap_refswatcher_rm(refswatcher, lc);
  }
}

static inline void rm_from_local_refs(CMAP_LIFECYCLE * lc)
{
  if(lc -> internal.ref_state != REF_STATE_FREE)
  {
    CMAP_PROC_CTX * proc_ctx = cmap_env_proc_ctx(lc -> internal.env);
    cmap_proc_ctx_local_refs_rm(proc_ctx, lc);
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_delete(CMAP_LIFECYCLE * lc)
{
  cmap_log_debug("[%p][%s] deletion", lc, lc -> internal.nature);

  rm_from_refswatcher(lc);
  rm_from_local_refs(lc);

  cmap_mem_free(lc);
}

CMAP_LIFECYCLE * cmap_lifecycle_init(CMAP_LIFECYCLE * lc,
  CMAP_INITARGS * initargs)
{
  CMAP_PROC_CTX * proc_ctx = initargs -> proc_ctx;

  lc -> internal.nature = initargs -> nature;
  lc -> internal.nb_refs = 0;
  lc -> internal.env = cmap_proc_ctx_env(proc_ctx);
  lc -> internal.watch_time_us = 0;
  lc -> internal.ref_state = REF_STATE_STORED;

  cmap_proc_ctx_local_refs_add(proc_ctx, lc, CMAP_T);

  lc -> delete = cmap_lifecycle_delete;
  lc -> nested = cmap_lifecycle_nested;

  cmap_log_debug("[%p][%s] creation", lc, lc -> internal.nature);

  return lc;
}
