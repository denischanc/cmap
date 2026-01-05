
#include "cmap-refsstore.h"

#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-sset.h"
#include "cmap-slist.h"
#include "cmap-log.h"
#include "cmap-lifecycle.h"

#ifdef CONSUMED_TIME
#include "cmap-consumedtime.h"
#endif

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SSET_LC * refs;

  int nb_created;

  CMAP_REFSWATCHER * refswatcher;
} INTERNAL;

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time = {0};
#endif

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_REFSSTORE * this, CMAP_LIFECYCLE * lc, char created)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  if(cmap_sset_lc_public.add(&internal -> refs, lc))
  {
    cmap_log_public.debug("[%p][refsstore] new ref : [%p]", this, lc);
    if(created) internal -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_REFSSTORE * this, CMAP_LIFECYCLE * lc)
{
  cmap_sset_lc_public.rm_v(&((INTERNAL *)(this + 1)) -> refs, lc);
}

/*******************************************************************************
*******************************************************************************/

static char delete_ref(INTERNAL * internal, CMAP_LIFECYCLE * lc)
{
  int nb_refs = CMAP_CALL(lc, nb_refs);
  if(nb_refs <= 0)
  {
    CMAP_DELETE(lc);
    return CMAP_T;
  }
  else
  {
    CMAP_CALL_ARGS(internal -> refswatcher, add, lc);
    return CMAP_F;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete_refs(INTERNAL * internal, CMAP_LIFECYCLE * ret)
{
  int nb_deleted = 0;

  CMAP_SSET_LC ** refs = &internal -> refs;
  while(*refs != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_sset_lc_public.rm(refs);
    char in_refs = CMAP_CALL(lc, in_refs);

    if(in_refs && (lc != ret) && delete_ref(internal, lc)) nb_deleted++;
  }

  cmap_log_public.debug("[%p][refsstore] deleted %d",
    ((CMAP_REFSSTORE *)internal) - 1, nb_deleted);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_REFSSTORE * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  cmap_log_public.debug("[%p][refsstore] created %d", this,
    internal -> nb_created);

#ifdef CONSUMED_TIME
  cmap_consumedtime_public.start(&consumed_time);
#endif
  delete_refs(internal, (CMAP_LIFECYCLE *)ret);
#ifdef CONSUMED_TIME
  cmap_consumedtime_public.stop(&consumed_time);
#endif

  CMAP_MEM_INSTANCE_FREE(this);
}

static CMAP_REFSSTORE * create(CMAP_REFSWATCHER * refswatcher)
{
  CMAP_REFSSTORE * this = (CMAP_REFSSTORE *)CMAP_MEM_INSTANCE -> alloc(
    sizeof(CMAP_REFSSTORE) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> refs = NULL;
  internal -> nb_created = 0;
  internal -> refswatcher = refswatcher;

  this -> delete = delete;
  this -> add = add;
  this -> rm = rm;

  return this;
}

/*******************************************************************************
*******************************************************************************/

#ifdef CONSUMED_TIME
static void log_consumed_time(char lvl)
{
  cmap_consumedtime_public.log(lvl, &consumed_time, "refsstore");
}
#endif

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSSTORE_PUBLIC cmap_refsstore_public =
{
  create,
#ifdef CONSUMED_TIME
  log_consumed_time
#endif
};
