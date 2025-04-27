
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

  CMAP_SLIST_LC * wrappers;

  CMAP_SLIST_LC_PTR * nesteds;
} REF_EXT;

static REF_EXT * create_ref_ext(CMAP_LIFECYCLE * lc)
{
  CMAP_KERNEL_ALLOC_PTR(ret, REF_EXT);

  ret -> lc = lc;
  ret -> wrappers = cmap_slist_lc_public.create(0);
  ret -> nesteds = cmap_slist_lc_ptr_public.create(0);

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

typedef struct
{
  char ret;
  CMAP_LIFECYCLE * cur;
  CMAP_SSET_REF_EXT ** all_ref_exts;
} UPD_ALL_REF_EXTS_APPLY_DATA;

static REF_EXT * upd_all_ref_exts(CMAP_LIFECYCLE * cur,
  CMAP_SSET_REF_EXT ** all_ref_exts);

static void upd_all_ref_exts_apply(CMAP_LIFECYCLE *** nested, void * data)
{
  UPD_ALL_REF_EXTS_APPLY_DATA * data_ = data;
  REF_EXT * ref_ext_nested = upd_all_ref_exts(**nested, data_ -> all_ref_exts);
  if(ref_ext_nested != NULL)
  {
    data_ -> ret = CMAP_T;

    CMAP_SLIST_LC * wrappers = ref_ext_nested -> wrappers;
    CMAP_CALL_ARGS(wrappers, push, data_ -> cur);
  }
}

static REF_EXT * upd_all_ref_exts(CMAP_LIFECYCLE * cur,
  CMAP_SSET_REF_EXT ** all_ref_exts)
{
  REF_EXT ref_ext_tmp;
  ref_ext_tmp.lc = cur;
  REF_EXT ** ref_ext_cur_ptr = ref_ext_get(*all_ref_exts, &ref_ext_tmp);
  if(ref_ext_cur_ptr != NULL) return *ref_ext_cur_ptr;

  REF_EXT * ref_ext_cur = create_ref_ext(cur);
  CMAP_SLIST_LC_PTR * nesteds = ref_ext_cur -> nesteds;
  CMAP_CALL_ARGS(cur, nested, nesteds);
  ref_ext_add_force(all_ref_exts, ref_ext_cur);

  UPD_ALL_REF_EXTS_APPLY_DATA data = {CMAP_F, cur, all_ref_exts};
  CMAP_APPLY(nesteds, upd_all_ref_exts_apply, &data);

  return (data.ret) ? ref_ext_cur : NULL;
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SSET_REF_EXT ** way_ref_exts, * all_ref_exts;
} UPD_WAY_REF_EXTS_APPLY_DATA;

static void upd_way_ref_exts(CMAP_SSET_REF_EXT ** way_ref_exts,
  REF_EXT * ref_ext, CMAP_SSET_REF_EXT * all_ref_exts);

static void upd_way_ref_exts_apply(CMAP_LIFECYCLE ** wrapper, void * data)
{
  UPD_WAY_REF_EXTS_APPLY_DATA * data_ = data;

  REF_EXT ref_ext_wrapper;
  ref_ext_wrapper.lc = *wrapper;
  upd_way_ref_exts(data_ -> way_ref_exts,
    *ref_ext_get(data_ -> all_ref_exts, &ref_ext_wrapper),
    data_ -> all_ref_exts);
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

static void check_way_ref_exts_apply(REF_EXT ** ref_ext, void * data)
{
  char * ret = data;
  if(*ret)
  {
    CMAP_LIFECYCLE * lc = (*ref_ext) -> lc;
    CMAP_SLIST_LC * wrappers = (*ref_ext) -> wrappers;
    int nb_refs = CMAP_CALL(lc, nb_refs),
      nb_wrappers = CMAP_CALL(wrappers, size);

    if(nb_refs > nb_wrappers) *ret = CMAP_F;
  }
}

static char check_way_ref_exts(CMAP_SSET_REF_EXT * way_ref_exts)
{
  char ret = CMAP_T;
  ref_ext_apply(way_ref_exts, check_way_ref_exts_apply, &ret);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char is_zombie_w_ref_exts(CMAP_LIFECYCLE * lc,
  CMAP_SSET_REF_EXT ** all_ref_exts, CMAP_SSET_REF_EXT ** way_ref_exts)
{
  REF_EXT * ref_ext = upd_all_ref_exts(lc, all_ref_exts);
  upd_way_ref_exts(way_ref_exts, ref_ext, *all_ref_exts);
  CMAP_CALL(lc, dec_refs_only);
  return check_way_ref_exts(*way_ref_exts);
}

static void delete_all_ref_exts(CMAP_SSET_REF_EXT ** all_ref_exts)
{
  while(*all_ref_exts != NULL) delete_ref_ext(ref_ext_rm(all_ref_exts));
}

/*******************************************************************************
*******************************************************************************/

static char is_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  CMAP_SSET_REF_EXT * all_ref_exts = NULL, * way_ref_exts = NULL;

  char ret = is_zombie_w_ref_exts(lc, &all_ref_exts, &way_ref_exts);

  ref_ext_clean(&way_ref_exts);
  delete_all_ref_exts(&all_ref_exts);

  CMAP_INC_REFS(lc);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete_zombie_apply_apply(CMAP_LIFECYCLE *** nested, void * data)
{
  CMAP_SSET_REF_EXT * way_ref_exts = data;
  CMAP_LIFECYCLE ** nested_ = *nested;

  REF_EXT ref_ext_nested;
  ref_ext_nested.lc = *nested_;
  if(ref_ext_is(way_ref_exts, &ref_ext_nested)) *nested_ = NULL;
}

static void delete_zombie_apply(REF_EXT ** ref_ext, void * data)
{
  CMAP_SSET_REF_EXT * way_ref_exts = data;

  CMAP_SLIST_LC_PTR * nesteds = (*ref_ext) -> nesteds;
  CMAP_APPLY(nesteds, delete_zombie_apply_apply, way_ref_exts);

  CMAP_LIFECYCLE * lc = (*ref_ext) -> lc;
  CMAP_DELETE(lc);
}

static void delete_zombie(CMAP_SSET_REF_EXT * way_ref_exts)
{
  ref_ext_apply(way_ref_exts, delete_zombie_apply, way_ref_exts);
}

/*******************************************************************************
*******************************************************************************/

static void delete_if_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  CMAP_SSET_REF_EXT * all_ref_exts = NULL, * way_ref_exts = NULL;
  if(is_zombie_w_ref_exts(lc, &all_ref_exts, &way_ref_exts))
    delete_zombie(way_ref_exts);

  ref_ext_clean(&way_ref_exts);
  delete_all_ref_exts(&all_ref_exts);
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
    else delete_if_zombie(this, ref.lc);
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
  this -> delete_if_zombie = delete_if_zombie;
  this -> watch = watch;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public =
{
  create
};
