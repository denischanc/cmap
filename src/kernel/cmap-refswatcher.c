
#include "cmap-refswatcher.h"

#include <stdio.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-config.h"
#include "cmap-util.h"
#include "cmap-sset.h"
#include "cmap-lifecycle.h"
#include "cmap-slist.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"
#include "cmap-loop-timer.h"

#ifdef CONSUMED_TIME
#include "cmap-consumedtime.h"
#endif

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;

  CMAP_SLIST_LC * wrappers;

  CMAP_SLIST_LC_PTR * nesteds;
} REF_EXT;

static REF_EXT * ref_ext_create(CMAP_LIFECYCLE * lc)
{
  CMAP_MEM_INSTANCE_ALLOC_PTR(ret, REF_EXT);

  ret -> lc = lc;
  ret -> wrappers = cmap_slist_lc_public.create(0);
  ret -> nesteds = cmap_slist_lc_ptr_public.create(0);

  return ret;
}

static void ref_ext_delete(REF_EXT * ref_ext)
{
  CMAP_DELETE(ref_ext -> wrappers);
  CMAP_DELETE(ref_ext -> nesteds);
  CMAP_MEM_INSTANCE_FREE(ref_ext);
}

static int64_t ref_ext_eval(REF_EXT * v_l, REF_EXT * v_r)
{
  return (void *)v_l -> lc - (void *)v_r -> lc;
}

static const char * ref_ext_log_v(REF_EXT * v)
{
  static char buffer[20];
  snprintf(buffer, sizeof(buffer), "%p", v -> lc);
  return buffer;
}

CMAP_SSET_STATIC(REF_EXT, ref_ext, REF_EXT *, ref_ext_eval, ref_ext_log_v)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_ENV * env;

  CMAP_SSET_LC * refs;

  CMAP_LOOP_TIMER timer;

  char deletion;
} INTERNAL;

typedef struct
{
  CMAP_REFSWATCHER * this;
  CMAP_LIFECYCLE * cur, * org;
  REF_EXT * ref_ext;
  CMAP_SSET_REF_EXT ** all_ref_exts, ** way_ref_exts;
  char ret;
} ZOMBIE_DATA;

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time_delete = {0};
static CMAP_CONSUMEDTIME_US consumed_time_watch = {0};
#endif

/*******************************************************************************
*******************************************************************************/

static void delete_if_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

static void add(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  if(internal -> deletion) delete_if_zombie(this, lc);
  else
  {
    if(!CMAP_CALL(lc, is_watched))
      cmap_sset_lc_public.add_force(&internal -> refs, lc);
    CMAP_CALL_ARGS(lc, watched, CMAP_T);
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
  cmap_sset_lc_public.rm_v(&((INTERNAL *)(this + 1)) -> refs, lc);
}

/*******************************************************************************
*******************************************************************************/

static REF_EXT * upd_all_ref_exts(ZOMBIE_DATA * data);

static void upd_all_ref_exts_apply(CMAP_LIFECYCLE *** nested, void * data)
{
  ZOMBIE_DATA * data_ = data, data_arg = *data_;
  data_arg.cur = **nested;
  REF_EXT * ref_ext_nested = upd_all_ref_exts(&data_arg);
  if(ref_ext_nested != NULL)
  {
    data_ -> ret = CMAP_T;

    CMAP_SLIST_LC * wrappers = ref_ext_nested -> wrappers;
    CMAP_CALL_ARGS(wrappers, push, data_ -> cur);
  }
}

static REF_EXT * upd_all_ref_exts(ZOMBIE_DATA * data)
{
  REF_EXT ref_ext_tmp;
  ref_ext_tmp.lc = data -> cur;
  REF_EXT ** ref_ext_cur_ptr = ref_ext_get(*data -> all_ref_exts, &ref_ext_tmp);
  if(ref_ext_cur_ptr != NULL) return *ref_ext_cur_ptr;

  REF_EXT * ref_ext_cur = ref_ext_create(data -> cur);
  CMAP_SLIST_LC_PTR * nesteds = ref_ext_cur -> nesteds;
  CMAP_CALL_ARGS(data -> cur, nested, nesteds);
  ref_ext_add_force(data -> all_ref_exts, ref_ext_cur);

  data -> ret = CMAP_F;
  CMAP_APPLY(nesteds, upd_all_ref_exts_apply, data);

  if(data -> ret) return ref_ext_cur;
  else
  {
    if((data -> cur != data -> org) && CMAP_CALL(data -> cur, is_watched))
    {
      rm(data -> this, data -> cur);
      CMAP_CALL_ARGS(data -> cur, watched, CMAP_F);
    }
    return NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

static void upd_way_ref_exts(ZOMBIE_DATA * data);

static void upd_way_ref_exts_apply(CMAP_LIFECYCLE ** wrapper, void * data)
{
  ZOMBIE_DATA * data_ = data, data_arg = *data_;

  REF_EXT ref_ext_wrapper;
  ref_ext_wrapper.lc = *wrapper;
  data_arg.ref_ext = *ref_ext_get(*data_ -> all_ref_exts, &ref_ext_wrapper);
  upd_way_ref_exts(&data_arg);
}

static void upd_way_ref_exts(ZOMBIE_DATA * data)
{
  REF_EXT * ref_ext = data -> ref_ext;
  if(ref_ext_add(data -> way_ref_exts, ref_ext))
  {
    CMAP_SLIST_LC * wrappers = ref_ext -> wrappers;
    CMAP_APPLY(wrappers, upd_way_ref_exts_apply, data);
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

static char is_zombie_w_ref_exts(ZOMBIE_DATA * data)
{
  REF_EXT * ref_ext = upd_all_ref_exts(data);
  if(ref_ext == NULL) return CMAP_F;

  data -> ref_ext = ref_ext;
  upd_way_ref_exts(data);
  return check_way_ref_exts(*data -> way_ref_exts);
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

static void delete_all_ref_exts(CMAP_SSET_REF_EXT ** all_ref_exts)
{
  while(*all_ref_exts != NULL) ref_ext_delete(ref_ext_rm(all_ref_exts));
}

static void delete_if_zombie(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc)
{
#ifdef CONSUMED_TIME
  cmap_consumedtime_public.start(&consumed_time_delete);
#endif

  CMAP_SSET_REF_EXT * all_ref_exts = NULL, * way_ref_exts = NULL;

  ZOMBIE_DATA data = {this, lc, lc, NULL, &all_ref_exts, &way_ref_exts, 0};
  if(is_zombie_w_ref_exts(&data))
  {
    cmap_log_public.debug("[%p][%s] zombie deletion", lc, CMAP_NATURE(lc));

    delete_zombie(way_ref_exts);

    cmap_log_public.debug("[%p][refswatcher] %d zombie(s) deleted", this,
      ref_ext_size(way_ref_exts));
  }

  ref_ext_clean(&way_ref_exts);
  delete_all_ref_exts(&all_ref_exts);

#ifdef CONSUMED_TIME
  cmap_consumedtime_public.stop(&consumed_time_delete);
#endif
}

/*******************************************************************************
*******************************************************************************/

static char watch_ref(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE ** ref,
  char deletion)
{
  CMAP_LIFECYCLE * ref_ = *ref;

  uint64_t now = cmap_util_time_us();
  if((CMAP_CALL(ref_, watch_time_us) < now) || deletion)
  {
    CMAP_CALL_ARGS(ref_, watched, CMAP_F);
    delete_if_zombie(this, ref_);
    return CMAP_T;
  }
  return CMAP_F;
}

static void watch(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> refs == NULL) return;

  cmap_log_public.debug("[%p][refswatcher] start watching", this);

#ifdef CONSUMED_TIME
  cmap_consumedtime_public.start(&consumed_time_watch);
#endif

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(internal -> env);

  CMAP_ITERATOR_SSET_LC * it = cmap_sset_lc_public.iterator(&internal -> refs);
  while(CMAP_CALL(it, has_next))
    if(watch_ref(this, CMAP_CALL(it, next), internal -> deletion))
      CMAP_CALL(it, rm);
  CMAP_DELETE(it);

  CMAP_CALL_ARGS(proc_ctx, delete, NULL);

#ifdef CONSUMED_TIME
  cmap_consumedtime_public.stop(&consumed_time_watch);
#endif

  cmap_log_public.debug("[%p][refswatcher] stop watching", this);
}

/*******************************************************************************
*******************************************************************************/

static void watch_loop(CMAP_LOOP_TIMER * timer)
{
  watch(timer -> data);
}

static inline void loop_init(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  uint64_t time_ms = cmap_config_refs_check_zombie_time_us() / 1000;

  internal -> timer.data = this;
  cmap_loop_timer_start(&internal -> timer, watch_loop, time_ms, time_ms);
}

static void stop(CMAP_REFSWATCHER * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  cmap_loop_timer_stop(&internal -> timer);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_REFSWATCHER * this)
{
  ((INTERNAL *)(this + 1)) -> deletion = CMAP_T;
  watch(this);

  CMAP_MEM_INSTANCE_FREE(this);

  cmap_log_public.debug("[%p][refswatcher] deleted", this);
}

CMAP_REFSWATCHER * cmap_refswatcher_create(CMAP_ENV * env)
{
  CMAP_REFSWATCHER * this = (CMAP_REFSWATCHER *)CMAP_MEM_INSTANCE -> alloc(
    sizeof(CMAP_REFSWATCHER) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> env = env;
  internal -> refs = NULL;
  internal -> deletion = CMAP_F;

  this -> delete = delete;
  this -> add = add;
  this -> rm = rm;
  this -> stop = stop;

  if(cmap_kernel_instance() -> state() != CMAP_KERNEL_S_EXITING)
    loop_init(this);

  cmap_log_public.debug("[%p][refswatcher] created", this);

  return this;
}

/*******************************************************************************
*******************************************************************************/

#ifdef CONSUMED_TIME
void cmap_refswatcher_log_consumed_time(char lvl)
{
  cmap_consumedtime_public.log(lvl, &consumed_time_delete,
    "refswatcher/delete");
  cmap_consumedtime_public.log(lvl, &consumed_time_watch, "refswatcher/watch");
}
#endif
