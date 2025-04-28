
#include "cmap-refsstore.h"

#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-sset.h"
#include "cmap-slist.h"
#include "cmap-log.h"
#include "cmap-lifecycle.h"
#include "cmap-env.h"
#include "cmap-refswatcher.h"

#ifdef CONSUMED_TIME
#include "cmap-consumedtime.h"
#endif

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SSET_LC * refs;

  int nb_created;

  CMAP_ENV * env;
} INTERNAL;

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time = {0};
#endif

/*******************************************************************************
*******************************************************************************/

static CMAP_REFSWATCHER * refswatcher(INTERNAL * internal)
{
  return CMAP_CALL(internal -> env, refswatcher);
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_REFSSTORE * this, CMAP_LIFECYCLE * lc, char created)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(cmap_sset_lc_public.add(&internal -> refs, lc))
  {
    cmap_log_public.debug("[%p][refsstore] new ref : [%p]", this, lc);

    CMAP_INC_REFS(lc);

    if(created) internal -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

static char delete_or_dec_refs_only(INTERNAL * internal, CMAP_LIFECYCLE * lc)
{
  int nb_refs = CMAP_CALL(lc, nb_refs);
  if(nb_refs <= 1)
  {
    CMAP_DELETE(lc);

    return CMAP_T;
  }
  else
  {
    cmap_log_public.debug("[%p][%s] nb_refs == [%d]", lc, CMAP_NATURE(lc),
      nb_refs);
    CMAP_CALL(lc, dec_refs_only);

    CMAP_REFSWATCHER * refswatcher_ = refswatcher(internal);
    CMAP_CALL_ARGS(refswatcher_, add, lc);

    return CMAP_F;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete_refs(INTERNAL * internal, CMAP_LIFECYCLE * ret)
{
  int nb_loop = 0, nb_deleted = 0, nb_ret = 0;

  CMAP_SSET_LC ** refs = &internal -> refs;
  while(*refs != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_sset_lc_public.rm(refs);
    if(lc == ret) nb_ret++;
    else
    {
      if(delete_or_dec_refs_only(internal, lc)) nb_deleted++;
      nb_loop++;
    }
  }

  cmap_log_public.debug("[refsstore] deleted %d/%d, nb ret = %d",
    nb_deleted, nb_loop, nb_ret);

  if(ret != NULL) CMAP_CALL_ARGS(ret, dec_refs_only_nb, nb_ret);
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

  CMAP_KERNEL_FREE(this);
}

static CMAP_REFSSTORE * create(CMAP_ENV * env)
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_REFSSTORE * this = (CMAP_REFSSTORE *)mem -> alloc(
    sizeof(CMAP_REFSSTORE) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> refs = NULL;
  internal -> nb_created = 0;
  internal -> env = env;

  this -> delete = delete;
  this -> add = add;

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
