
#include "cmap-refswatcher.h"

#include <uv.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-util.h"
#include "cmap-sset.h"
#include "cmap-lifecycle.h"
#include "cmap-slist.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;

  uint64_t time_us;
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
  static char buffer[20];
  snprintf(buffer, sizeof(buffer), "%p", v.lc);
  return buffer;
}

CMAP_SSET_STATIC(REF, ref, REF, ref_eval, ref_log_v)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_ENV * env;

  uint64_t time_us;

  CMAP_SSET_REF * refs;

  uv_timer_t handle;
  char stopped;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static char delete_if_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

static char add(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  if(internal -> stopped)
  {
    CMAP_INC_REFS(lc);
    return delete_if_zombie(this, lc);
  }
  else
  {
    REF ref;
    ref.lc = lc;
    if(CMAP_CALL(lc, is_watched))
    {
      REF * ref_ = ref_get(internal -> refs, ref);
      ref_ -> time_us = cmap_util_public.time_us() + internal -> time_us;
    }
    else
    {
      ref.time_us = cmap_util_public.time_us() + internal -> time_us;
      ref_add_force(&internal -> refs, ref);
      CMAP_INC_REFS(lc);
      CMAP_CALL_ARGS(lc, watched, CMAP_T);
    }
    return CMAP_F;
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  REF ref;
  ref.lc = lc;
  if(ref_rm_v(&internal -> refs, ref))
  {
    CMAP_CALL(lc, dec_refs_only);
    CMAP_CALL_ARGS(lc, watched, CMAP_F);
  }
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
  CMAP_CALL(lc, dec_refs_only);

  REF_EXT * ref_ext = upd_all_ref_exts(lc, all_ref_exts);
  if(ref_ext == NULL) return CMAP_F;

  upd_way_ref_exts(way_ref_exts, ref_ext, *all_ref_exts);
  return check_way_ref_exts(*way_ref_exts);
}

static void delete_all_ref_exts(CMAP_SSET_REF_EXT ** all_ref_exts)
{
  while(*all_ref_exts != NULL) delete_ref_ext(ref_ext_rm(all_ref_exts));
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

static char delete_if_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  char ret = CMAP_F;

  CMAP_SSET_REF_EXT * all_ref_exts = NULL, * way_ref_exts = NULL;
  if(is_zombie_w_ref_exts(lc, &all_ref_exts, &way_ref_exts))
  {
    ret = CMAP_T;

    cmap_log_public.debug("[%p][%s] zombie deletion", lc, CMAP_NATURE(lc));
    delete_zombie(way_ref_exts);
    cmap_log_public.debug("[%p][refswatcher] %d zombie(s) deleted", this,
      ref_ext_size(way_ref_exts));
  }
  ref_ext_clean(&way_ref_exts);
  delete_all_ref_exts(&all_ref_exts);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static REF ref_l_dft = {NULL, 0};
CMAP_SLIST_STATIC(REF_L, ref_l, REF, ref_l_dft)

static void watch_fill_apply(REF * ref, void * data)
{
  CMAP_SLIST_REF_L * refs = data;
  CMAP_CALL_ARGS(refs, push, *ref);
}

typedef struct
{
  CMAP_SSET_REF ** refs;
  CMAP_REFSWATCHER * this;
  char stopped;
} WATCH_PROC_APPLY_DATA;

static void watch_proc_apply(REF * ref, void * data)
{
  WATCH_PROC_APPLY_DATA * data_ = data;

  if(ref_is(*data_ -> refs, *ref))
  {
    uint64_t now = cmap_util_public.time_us();
    if((ref -> time_us > now) || data_ -> stopped)
    {
      ref_rm_v(data_ -> refs, *ref);
      if(!delete_if_zombie(data_ -> this, ref -> lc))
        CMAP_CALL_ARGS(ref -> lc, watched, CMAP_F);
    }
  }
}

static void watch(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  CMAP_SLIST_REF_L * refs = ref_l_create(0);
  ref_apply(internal -> refs, watch_fill_apply, refs);

  WATCH_PROC_APPLY_DATA data = {&internal -> refs, this, internal -> stopped};
  CMAP_APPLY(refs, watch_proc_apply, &data);

  CMAP_DELETE(refs);
}

/*******************************************************************************
*******************************************************************************/

static void watch_uv(uv_timer_t * handle)
{
  CMAP_REFSWATCHER * this = handle -> data;
  INTERNAL * internal = (INTERNAL *)(this + 1);

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(internal -> env);
  watch(this);
  CMAP_CALL_ARGS(proc_ctx, delete, NULL);
}

static void this_uv_init(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  internal -> handle.data = this;

  cmap_util_public.uv_error(uv_timer_init(CMAP_KERNEL_INSTANCE -> uv_loop(),
    &internal -> handle));
  cmap_util_public.uv_error(uv_timer_start(&internal -> handle, watch_uv, 0,
    internal -> time_us));
}

/*******************************************************************************
*******************************************************************************/

static void stop(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  internal -> stopped = CMAP_T;

  cmap_util_public.uv_error(uv_timer_stop(&internal -> handle));

  watch(this);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_REFSWATCHER * this)
{
  CMAP_KERNEL_FREE(this);
}

static CMAP_REFSWATCHER * create(CMAP_ENV * env)
{
  CMAP_KERNEL * kernel = CMAP_KERNEL_INSTANCE;
  CMAP_MEM * mem = kernel -> mem();
  CMAP_REFSWATCHER * this = (CMAP_REFSWATCHER *)mem -> alloc(
    sizeof(CMAP_REFSWATCHER) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> env = env;
  internal -> time_us = kernel -> cfg() -> check_zombie_time_us;
  internal -> refs = NULL;
  internal -> stopped = CMAP_F;

  this -> delete = delete;
  this -> add = add;
  this -> rm = rm;
  this -> stop = stop;

  this_uv_init(this);

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public = { create };
