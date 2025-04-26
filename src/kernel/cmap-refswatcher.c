
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

static int ref_eval(REF v_l, REF v_r)
{
  CMAP_LIFECYCLE * ref_l = v_l.lc;
  CMAP_LIFECYCLE * ref_r = v_r.lc;

  if(ref_l > ref_r) return 1;
  else if(ref_l < ref_r) return -1;
  else return 0;
}

static const char * ref_log_v(REF v)
{
  return NULL;
}

CMAP_SSET_STATIC(REF, ref, REF, ref_eval, ref_log_v)

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

  CMAP_SLIST_LC * wrappers, * nesteds;
} REF_EXT;

static REF_EXT * create_ref_ext(CMAP_LIFECYCLE * lc)
{
  CMAP_KERNEL_ALLOC_PTR(ret, REF_EXT);

  ret -> lc = lc;
  ret -> wrappers = cmap_slist_lc_public.create(0);
  ret -> nesteds = cmap_slist_lc_public.create(0);

  return ret;
}

static void delete_ref_ext(REF_EXT * ref_ext)
{
  CMAP_DELETE(ref_ext -> wrappers);
  CMAP_DELETE(ref_ext -> nesteds);
  CMAP_KERNEL_FREE(ref_ext);
}

/*******************************************************************************
*******************************************************************************/

static int ref_ext_eval(REF_EXT * v_l, REF_EXT * v_r)
{
  CMAP_LIFECYCLE * ref_l = v_l -> lc;
  CMAP_LIFECYCLE * ref_r = v_r -> lc;

  if(ref_l > ref_r) return 1;
  else if(ref_l < ref_r) return -1;
  else return 0;
}

static const char * ref_ext_log_v(REF_EXT * v)
{
  return NULL;
}

CMAP_SSET_STATIC(REF_EXT, ref_ext, REF_EXT *, ref_ext_eval, ref_ext_log_v)

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
    REF_EXT ** ref_ext_nested_ptr = ref_ext_get(*all_ref_exts, &ref_ext_nested);
    if(ref_ext_nested_ptr != NULL)
    {
      CMAP_SLIST_LC * wrappers = (*ref_ext_nested_ptr) -> wrappers;
      CMAP_CALL_ARGS(wrappers, push, cur);
    }
    else
    {
      REF_EXT * ref_ext_new = create_ref_ext(*nested);
      ref_ext_add_force(all_ref_exts, ref_ext_new);

      CMAP_SLIST_LC * wrappers = ref_ext_new -> wrappers;
      CMAP_CALL_ARGS(wrappers, push, cur);

      upd_all_ref_exts(*nested, org, all_ref_exts);
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

static void upd_way_ref_exts_apply(CMAP_LIFECYCLE ** lc, void * data)
{
  UPD_WAY_REF_EXTS_APPLY_DATA * data_ = data;

  REF_EXT ref_ext;
  ref_ext.lc = *lc;
  upd_way_ref_exts(data_ -> way_ref_exts,
    *ref_ext_get(data_ -> all_ref_exts, &ref_ext), data_ -> all_ref_exts);
}

static void upd_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  REF_EXT * ref_ext, CMAP_SSET_REF_EXT * all_ref_exts)
{
  if(ref_ext_add(way_ref_exts, ref_ext))
  {
    UPD_WAY_REF_EXTS_APPLY_DATA data;
    data.way_ref_exts = way_ref_exts;
    data.all_ref_exts = all_ref_exts;
    CMAP_SLIST_LC * wrappers = ref_ext -> wrappers;
    CMAP_APPLY(wrappers, upd_way_ref_exts_apply, &data);
  }
}

/*******************************************************************************
*******************************************************************************/

static char check_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  CMAP_LIFECYCLE * org)
{
  char ret = CMAP_T;

  while(ret && (*way_ref_exts != NULL))
  {
    REF_EXT * ref_ext = ref_ext_rm(way_ref_exts);

    CMAP_LIFECYCLE * lc = ref_ext -> lc;
    CMAP_SLIST_LC * wrappers = ref_ext -> wrappers;
    int nb_wrappers = CMAP_CALL(wrappers, size),
      nb_refs = CMAP_CALL(lc, nb_refs);

    if(lc == org)
    {
      if(nb_refs > (nb_wrappers + 1)) ret = CMAP_F;
    }
    else if(nb_refs > nb_wrappers) ret = CMAP_F;
  }

  ref_ext_clean(way_ref_exts);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char is_zombie_w_all_ref_exts(CMAP_LIFECYCLE * lc,
  CMAP_SSET_REF_EXT ** all_ref_exts)
{
  REF_EXT * org = create_ref_ext(lc);

  ref_ext_add(all_ref_exts, org);
  upd_all_ref_exts(lc, lc, all_ref_exts);

  CMAP_SSET_REF_EXT * way_ref_exts = NULL;
  upd_way_ref_exts(&way_ref_exts, org, *all_ref_exts);

  return check_way_ref_exts(&way_ref_exts, lc);
}

static void delete_all_ref_exts(CMAP_SSET_REF_EXT ** all_ref_exts)
{
  while(*all_ref_exts != NULL)
  {
    REF_EXT * ref_ext = ref_ext_rm(all_ref_exts);
    delete_ref_ext(ref_ext);
  }
}

static char is_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  CMAP_SSET_REF_EXT * all_ref_exts = NULL;
  char ret = is_zombie_w_all_ref_exts(lc, &all_ref_exts);

  delete_all_ref_exts(&all_ref_exts);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char delete_if_zombie(CMAP_LIFECYCLE * lc)
{
  char ret = CMAP_F;

  CMAP_SSET_REF_EXT * all_ref_exts = NULL;
  if(is_zombie_w_all_ref_exts(lc, &all_ref_exts))
  {
    ret = CMAP_T;
    /* TODO : delete */
  }

  delete_all_ref_exts(&all_ref_exts);

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
  this -> is_zombie = is_zombie;
  this -> watch = watch;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public =
{
  create
};
