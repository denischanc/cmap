
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

struct CMAP_REFSSTORE
{
  CMAP_SSET_LC * refs;

  int nb_created;

  CMAP_REFSWATCHER * refswatcher;
};

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time = {0};
#endif

/*******************************************************************************
*******************************************************************************/

void cmap_refsstore_add(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * lc, char created)
{
  if(cmap_sset_lc_public.add(&rs -> refs, lc))
  {
    cmap_log_debug("[%p][refsstore] new ref : [%p]", rs, lc);
    if(created) rs -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_refsstore_rm(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * lc)
{
  cmap_sset_lc_public.rm_v(&rs -> refs, lc);
}

/*******************************************************************************
*******************************************************************************/

static char delete_ref(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * lc)
{
  int nb_refs = CMAP_CALL(lc, nb_refs);
  if(nb_refs <= 0)
  {
    CMAP_DELETE(lc);
    return CMAP_T;
  }
  else
  {
    cmap_refswatcher_add(rs -> refswatcher, lc);
    return CMAP_F;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete_refs(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * ret)
{
  int nb_deleted = 0;

  CMAP_SSET_LC ** refs = &rs -> refs;
  while(*refs != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_sset_lc_public.rm(refs);
    char in_refs = CMAP_CALL(lc, in_refs);

    if(in_refs && (lc != ret) && delete_ref(rs, lc)) nb_deleted++;
  }

  cmap_log_debug("[%p][refsstore] deleted %d", rs, nb_deleted);
}

/*******************************************************************************
*******************************************************************************/

void cmap_refsstore_delete(CMAP_REFSSTORE * rs, CMAP_MAP * ret)
{
  cmap_log_debug("[%p][refsstore] created %d", rs, rs -> nb_created);

#ifdef CONSUMED_TIME
  cmap_consumedtime_start(&consumed_time);
#endif
  delete_refs(rs, (CMAP_LIFECYCLE *)ret);
#ifdef CONSUMED_TIME
  cmap_consumedtime_stop(&consumed_time);
#endif

  cmap_mem_free(rs);
}

CMAP_REFSSTORE * cmap_refsstore_create(CMAP_REFSWATCHER * refswatcher)
{
  CMAP_MEM_ALLOC_PTR(rs, CMAP_REFSSTORE);

  rs -> refs = NULL;
  rs -> nb_created = 0;
  rs -> refswatcher = refswatcher;

  return rs;
}

/*******************************************************************************
*******************************************************************************/

#ifdef CONSUMED_TIME
void cmap_refsstore_log_consumed_time(char lvl)
{
  cmap_consumedtime_log(lvl, &consumed_time, "refsstore");
}
#endif
