
#include "cmap-lifecycle.h"

#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"
#include "cmap-log.h"
#include "cmap-util.h"
#include "cmap-config.h"
#include "cmap-core.h"

/*******************************************************************************
*******************************************************************************/

#define REF_STATE_FREE 0
#define REF_STATE_STORED 1
#define REF_STATE_HOOKED 2

static const char * REF_STATES[] = {"free", "stored", "hooked"};

/*******************************************************************************
*******************************************************************************/

unsigned char cmap_lifecycle_nature(CMAP_LIFECYCLE * lc)
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

static inline void set_ref_state(CMAP_LIFECYCLE * lc, unsigned char ref_state)
{
  lc -> internal.ref_state = ref_state;

  cmap_log_debug("[%p][%s] ref_state = [%s], nb_refs = [%d]", lc,
    CMAP_NATURE_CHAR(lc), REF_STATES[ref_state], lc -> internal.nb_refs);
}

/*******************************************************************************
*******************************************************************************/

static inline void do_store(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
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

void cmap_lifecycle_dec_refs(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  lc -> internal.nb_refs--;

  if(lc -> internal.ref_state == REF_STATE_FREE) do_store(lc, proc_ctx);
  else if(lc -> internal.ref_state == REF_STATE_HOOKED)
    set_ref_state(lc, REF_STATE_STORED);
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_watched(CMAP_LIFECYCLE * lc, char val)
{
  if(val)
  {
    if(lc -> internal.watch_time_us <= 0)
      cmap_log_debug("[%p][%s] is watched", lc, CMAP_NATURE_CHAR(lc));

    lc -> internal.watch_time_us = next_watch_time_us();
  }
  else
  {
    cmap_log_debug("[%p][%s] not watched", lc, CMAP_NATURE_CHAR(lc));

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

void cmap_lifecycle_store(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  if(lc -> internal.ref_state == REF_STATE_FREE) do_store(lc, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

char cmap_lifecycle_in_refs(CMAP_LIFECYCLE * lc)
{
  unsigned char ref_state = lc -> internal.ref_state;

  set_ref_state(lc, REF_STATE_FREE);

  return (ref_state != REF_STATE_HOOKED);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_from_refswatcher(CMAP_LIFECYCLE * lc,
  CMAP_PROC_CTX * proc_ctx)
{
  if(lc -> internal.watch_time_us > 0)
  {
    CMAP_REFSWATCHER * refswatcher = cmap_proc_ctx_refswatcher(proc_ctx);
    cmap_refswatcher_rm(refswatcher, lc);
  }
}

static inline void rm_from_local_refs(CMAP_LIFECYCLE * lc,
  CMAP_PROC_CTX * proc_ctx)
{
  if(lc -> internal.ref_state != REF_STATE_FREE)
    cmap_proc_ctx_local_refs_rm(proc_ctx, lc);
}

/*******************************************************************************
*******************************************************************************/

void cmap_lifecycle_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx)
{
  cmap_log_debug("[%p][%s] deletion", lc, CMAP_NATURE_CHAR(lc));

  rm_from_refswatcher(lc, proc_ctx);
  rm_from_local_refs(lc, proc_ctx);

  cmap_mem_free(lc);
}

CMAP_LIFECYCLE * cmap_lifecycle_init(CMAP_LIFECYCLE * lc,
  CMAP_INITARGS * initargs)
{
  CMAP_PROC_CTX * proc_ctx = initargs -> proc_ctx;

  lc -> internal.nature = initargs -> nature;
  lc -> internal.nb_refs = 0;
  lc -> internal.watch_time_us = 0;
  lc -> internal.ref_state = REF_STATE_STORED;

  cmap_proc_ctx_local_refs_add(proc_ctx, lc, CMAP_T);

  cmap_log_debug("[%p][%s] creation", lc, CMAP_NATURE_CHAR(lc));

  return lc;
}
