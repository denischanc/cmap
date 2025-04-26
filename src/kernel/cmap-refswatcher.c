
#include "cmap-refswatcher.h"

#include "cmap-kernel.h"
#include "cmap-util.h"
#include "cmap-sset.h"
#include "cmap-lifecycle.h"
#include "cmap-slist.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;

  int64_t time;
} REF;

static int CMAP_STREE_EVALFN_NAME(ref)(void * node, void * data);

CMAP_SSET_STATIC(REF, ref, REF, NULL)

static int CMAP_STREE_EVALFN_NAME(ref)(void * node, void * data)
{
  CMAP_LIFECYCLE * ref_l = ((CMAP_SSET_REF *)node) -> v.lc;
  CMAP_LIFECYCLE * ref_r = ((CMAP_SSET_REF *)data) -> v.lc;

  if(ref_l > ref_r) return 1;
  else if(ref_l < ref_r) return -1;
  else return 0;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int64_t time_us;

  CMAP_SSET_REF * refs;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  REF ref;
  ref.lc = lc;
  ref.time = cmap_util_public.time_us() + internal -> time_us;
  if(ref_add(&internal -> refs, ref)) CMAP_INC_REFS(lc);
}

/*******************************************************************************
*******************************************************************************/

static void upd(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  REF ref;
  ref.lc = lc;
  ref.time = 0;
  REF * ref_ = ref_get(internal -> refs, ref);
  ref_ -> time = cmap_util_public.time_us() + internal -> time_us;
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  REF ref;
  ref.lc = lc;
  if(ref_rm_v(&internal -> refs, ref)) CMAP_CALL(lc, dec_refs_only);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;

  CMAP_SLIST_LC * wrappers;
} REF_EXT;

static REF_EXT create_ref_ext(CMAP_LIFECYCLE * lc)
{
  REF_EXT ret;
  ret.lc = lc;
  ret.wrappers = cmap_slist_lc_public.create(0);
  return ret;
}

static void delete_ref_ext(REF_EXT * ref_ext)
{
  CMAP_DELETE(ref_ext -> wrappers);
}

/*******************************************************************************
*******************************************************************************/

static int CMAP_STREE_EVALFN_NAME(ref_ext)(void * node, void * data);

CMAP_SSET_STATIC(REF_EXT, ref_ext, REF_EXT, NULL)

static int CMAP_STREE_EVALFN_NAME(ref_ext)(void * node, void * data)
{
  CMAP_LIFECYCLE * ref_l = ((CMAP_SSET_REF_EXT *)node) -> v.lc;
  CMAP_LIFECYCLE * ref_r = ((CMAP_SSET_REF_EXT *)data) -> v.lc;

  if(ref_l > ref_r) return 1;
  else if(ref_l < ref_r) return -1;
  else return 0;
}

/*******************************************************************************
*******************************************************************************/

static void upd_all_ref_exts(CMAP_LIFECYCLE * cur, CMAP_LIFECYCLE * org,
  CMAP_SSET_REF_EXT ** all_ref_exts)
{
  CMAP_SLIST_LC_PTR * nesteds = cmap_slist_lc_ptr_public.create(0);

  CMAP_CALL_ARGS(cur, nested, nesteds);

  CMAP_LIFECYCLE ** nested;
  while((nested = CMAP_CALL(nesteds, pop)) != NULL)
  {
    REF_EXT ref_ext_nested;
    ref_ext_nested.lc = *nested;
    REF_EXT * ref_ext_nested_ptr = ref_ext_get(*all_ref_exts, ref_ext_nested);
    if(ref_ext_nested_ptr != NULL)
      CMAP_CALL_ARGS(ref_ext_nested_ptr -> wrappers, push, cur);
    else
    {
      ref_ext_nested.wrappers = cmap_slist_lc_public.create(0);
      ref_ext_add_force(all_ref_exts, ref_ext_nested);
      CMAP_CALL_ARGS(ref_ext_nested.wrappers, push, cur);

      if(*nested != org) upd_all_ref_exts(*nested, org, all_ref_exts);
    }
  }

  CMAP_DELETE(nesteds);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SSET_REF_EXT ** way_ref_exts, * all_ref_exts;
} UPD_WAY_REF_EXTS_APPLY_DATA;

static void upd_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  REF_EXT * ref_ext, CMAP_SSET_REF_EXT * all_ref_exts);

static void upd_way_ref_exts_apply_fn(CMAP_LIFECYCLE ** lc, void * data)
{
  UPD_WAY_REF_EXTS_APPLY_DATA * data_ = (UPD_WAY_REF_EXTS_APPLY_DATA *)data;

  REF_EXT ref_ext;
  ref_ext.lc = *lc;
  ref_ext.wrappers = NULL;
  upd_way_ref_exts(data_ -> way_ref_exts,
    ref_ext_get(data_ -> all_ref_exts, ref_ext), data_ -> all_ref_exts);
}

static void upd_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  REF_EXT * ref_ext, CMAP_SSET_REF_EXT * all_ref_exts)
{
  if(ref_ext_add(way_ref_exts, *ref_ext))
  {
    UPD_WAY_REF_EXTS_APPLY_DATA data;
    data.way_ref_exts = way_ref_exts;
    data.all_ref_exts = all_ref_exts;
    CMAP_APPLY(ref_ext -> wrappers, upd_way_ref_exts_apply_fn, &data);
  }
}

/*******************************************************************************
*******************************************************************************/

static char check_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  CMAP_LIFECYCLE * org)
{
  while(*way_ref_exts != NULL)
  {
    REF_EXT ref_ext = ref_ext_rm(way_ref_exts);
    int nb_wrappers = CMAP_CALL(ref_ext.wrappers, size),
      nb_refs = CMAP_CALL(ref_ext.lc, nb_refs);

    if(ref_ext.lc == org)
    {
      if(nb_refs > (nb_wrappers + 1)) return CMAP_F;
    }
    else if(nb_refs > nb_wrappers) return CMAP_F;
  }
  return CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

static char delete_if_zombie(CMAP_LIFECYCLE * lc)
{
  REF_EXT org = create_ref_ext(lc);

  CMAP_SSET_REF_EXT * all_ref_exts = NULL;
  ref_ext_add(&all_ref_exts, org);
  upd_all_ref_exts(lc, lc, &all_ref_exts);

  CMAP_SSET_REF_EXT * way_ref_exts = NULL;
  upd_way_ref_exts(&way_ref_exts, &org, all_ref_exts);

  char ret = CMAP_F;

  if(check_way_ref_exts(&way_ref_exts, lc))
  {
    ret = CMAP_T;
    /* delete */
  }

  while(all_ref_exts != NULL)
  {
    REF_EXT ref_ext = ref_ext_rm(&all_ref_exts);
    delete_ref_ext(&ref_ext);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void watch(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_SSET_REF * refs = NULL;

  while(internal -> refs != NULL)
  {
    REF ref = ref_rm(&internal -> refs);

    int64_t now = cmap_util_public.time_us();
    if(ref.time <= now) ref_add(&refs, ref);
    else if(!delete_if_zombie(ref.lc)) CMAP_CALL(ref.lc, dec_refs_only);
  }

  internal -> refs = refs;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_REFSWATCHER * this)
{
  CMAP_KERNEL_FREE(this);
}

static CMAP_REFSWATCHER * create()
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_REFSWATCHER * this = (CMAP_REFSWATCHER *)mem -> alloc(
    sizeof(CMAP_REFSWATCHER) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> time_us = CMAP_KERNEL_INSTANCE -> cfg() -> check_zombie_time_us;
  internal -> refs = NULL;

  this -> delete = delete;
  this -> add = add;
  this -> upd = upd;
  this -> rm = rm;
  this -> watch = watch;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public =
{
  create
};
