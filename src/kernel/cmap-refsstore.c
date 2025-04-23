
#include "cmap-refsstore.h"

#include "cmap.h"
#include "cmap-kernel.h"
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
  CMAP_SSET_lc * refs;

  int nb_created;
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

    CMAP_INC_REFS(lc);

    if(created) internal -> nb_created++;
  }
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char way_ok;
  CMAP_LIFECYCLE * org;
  CMAP_SLIST_LC_PTR * crefs;
  CMAP_SSET_lc ** visiteds;
  CMAP_SSET_lc ** way_refs;
} CAT_CREFS_APPLY_DATA;

static char cat_cycling_refs(CMAP_LIFECYCLE * lc, CMAP_LIFECYCLE * org,
  CMAP_SLIST_LC_PTR * crefs, CMAP_SSET_lc ** visiteds,
  CMAP_SSET_lc ** way_refs);

static void cat_crefs_apply(CMAP_LIFECYCLE *** lc, void * data)
{
  CAT_CREFS_APPLY_DATA * data_ = (CAT_CREFS_APPLY_DATA *)data;
  if(**lc == data_ -> org)
  {
    CMAP_CALL_ARGS(data_ -> crefs, push, *lc);
    data_ -> way_ok = CMAP_T;
  }
  else
  {
    if(cat_cycling_refs(**lc, data_ -> org, data_ -> crefs, data_ -> visiteds,
        data_ -> way_refs))
      data_ -> way_ok = CMAP_T;
  }
}

static char cat_cycling_refs(CMAP_LIFECYCLE * lc, CMAP_LIFECYCLE * org,
  CMAP_SLIST_LC_PTR * crefs, CMAP_SSET_lc ** visiteds,
  CMAP_SSET_lc ** way_refs)
{
  if(cmap_sset_lc_public.add(visiteds, lc))
  {
    CMAP_SLIST_LC_PTR * nested_list = cmap_slist_lc_ptr_public.create(0);
    CMAP_CALL_ARGS(lc, nested, nested_list);
    CAT_CREFS_APPLY_DATA data = { CMAP_F, org, crefs, visiteds, way_refs };
    CMAP_APPLY(nested_list, cat_crefs_apply, &data);
    CMAP_DELETE(nested_list);

    if(data.way_ok)
    {
      cmap_sset_lc_public.add(way_refs, lc);
      return CMAP_T;
    }
    return CMAP_F;
  }

  return cmap_sset_lc_public.is(*way_refs, lc);
}

static CMAP_SLIST_LC_PTR * get_cycling_refs(CMAP_LIFECYCLE * lc)
{
  CMAP_SLIST_LC_PTR * crefs = cmap_slist_lc_ptr_public.create(0);

  CMAP_SSET_lc * visiteds = NULL, * way_refs = NULL;
  cat_cycling_refs(lc, lc, crefs, &visiteds, &way_refs);
  if(visiteds != NULL)
  {
    cmap_sset_lc_public.clean(&visiteds);
    if(way_refs != NULL) cmap_sset_lc_public.clean(&way_refs);
  }

  return crefs;
}

/*******************************************************************************
*******************************************************************************/

static char stay_future_zombie(CMAP_LIFECYCLE * way_ref)
{
  CMAP_SLIST_LC_PTR * crefs = get_cycling_refs(way_ref);
  int crefs_size = CMAP_CALL(crefs, size);
  CMAP_DELETE(crefs);
  return (CMAP_CALL(way_ref, nb_refs) <= crefs_size);
}

static void zombie_crefs_apply(CMAP_LIFECYCLE *** lc, void * data)
{
  **lc = NULL;
}

static char delete_future_zombie_required(CMAP_LIFECYCLE * lc, int nb_refs)
{
  char ret = CMAP_T;

  CMAP_SLIST_LC_PTR * crefs = cmap_slist_lc_ptr_public.create(0);
  CMAP_SSET_lc * visiteds = NULL, * way_refs = NULL;
  cat_cycling_refs(lc, lc, crefs, &visiteds, &way_refs);
  if(visiteds != NULL) cmap_sset_lc_public.clean(&visiteds);

  if(nb_refs > (CMAP_CALL(crefs, size) + 1)) ret = CMAP_F;
  else
  {
    while(ret && (way_refs != NULL))
    {
      CMAP_LIFECYCLE * way_ref = cmap_sset_lc_public.rm(&way_refs);
      if((way_ref != lc) && !stay_future_zombie(way_ref)) ret = CMAP_F;
    }
  }

  if(way_refs != NULL) cmap_sset_lc_public.clean(&way_refs);

  if(ret) CMAP_CALL_ARGS(crefs, apply, zombie_crefs_apply, NULL);
  CMAP_DELETE(crefs);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char delete_or_dec_refs_only(CMAP_LIFECYCLE * lc, char delete_zombie)
{
  char ret = CMAP_F;

  int nb_refs = CMAP_CALL(lc, nb_refs);
  if(nb_refs <= 1) ret = CMAP_T;
  else if(delete_zombie && delete_future_zombie_required(lc, nb_refs))
  {
    cmap_log_public.debug("[%p][%s] is zombie", lc, CMAP_NATURE(lc));
    ret = CMAP_T;
  }

  if(ret) CMAP_DELETE(lc);
  else
  {
    cmap_log_public.debug("[%p][%s] nb_refs == [%d]", lc, CMAP_NATURE(lc),
      nb_refs);
    CMAP_CALL(lc, dec_refs_only);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete_refs(INTERNAL * internal, CMAP_LIFECYCLE * ret)
{
  int nb_loop = 0, nb_deleted = 0, nb_ret = 0;
  char delete_zombie = CMAP_KERNEL_INSTANCE -> cfg() -> delete_zombie;

  CMAP_SSET_lc ** refs = &internal -> refs;
  while(*refs != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_sset_lc_public.rm(refs);
    if(lc == ret) nb_ret++;
    else
    {
      if(delete_or_dec_refs_only(lc, delete_zombie)) nb_deleted++;
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

static CMAP_REFSSTORE * create()
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_REFSSTORE * this = (CMAP_REFSSTORE *)mem -> alloc(
    sizeof(CMAP_REFSSTORE) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> refs = NULL;
  internal -> nb_created = 0;

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
