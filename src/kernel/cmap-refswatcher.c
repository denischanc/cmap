
#include "cmap-refswatcher.h"

#include <stdio.h>
#include <stdint.h>
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
#include "cmap-core.h"

#ifdef CONSUMED_TIME
#include "cmap-consumedtime.h"
#endif

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;
  uint64_t watch_time_us;
} REF;

static int64_t ref_eval(REF * v_l, REF * v_r)
{
  return (void *)v_l -> lc - (void *)v_r -> lc;
}

static const char * ref_log(REF * v)
{
  static char buffer[20];
  snprintf(buffer, sizeof(buffer), "%p", v -> lc);
  return buffer;
}

CMAP_SSET(REF, ref, REF, ref_eval, ref_log)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIFECYCLE * lc;

  CMAP_SLIST_LC * wrappers;

  CMAP_SLIST_LC_PTR * nesteds;
} REF_EXT;

static inline void ref_ext_init(REF_EXT * ref_ext)
{
  ref_ext -> wrappers = cmap_slist_lc_create(0);
  ref_ext -> nesteds = cmap_slist_lc_ptr_create(0);
}

static inline void ref_ext_delete(REF_EXT ref_ext)
{
  cmap_slist_lc_delete(ref_ext.wrappers);
  cmap_slist_lc_ptr_delete(ref_ext.nesteds);
}

static int64_t ref_ext_eval(REF_EXT * v_l, REF_EXT * v_r)
{
  return (void *)v_l -> lc - (void *)v_r -> lc;
}

static const char * ref_ext_log(REF_EXT * v)
{
  static char buffer[20];
  snprintf(buffer, sizeof(buffer), "%p", v -> lc);
  return buffer;
}

CMAP_SSET(REF_EXT, ref_ext, REF_EXT, ref_ext_eval, ref_ext_log)

/*******************************************************************************
*******************************************************************************/

struct CMAP_REFSWATCHER
{
  CMAP_ENV * env;
  uint64_t check_zombie_time_us;

  CMAP_SSET_REF * refs;

  CMAP_LOOP_TIMER timer;

  char deletion;
};

typedef struct
{
  CMAP_REFSWATCHER * rw;
  CMAP_LIFECYCLE * cur, * org;
  REF_EXT * ref_ext;
  CMAP_SSET_REF_EXT ** all_ref_exts, ** way_ref_exts;
  char ret;
  CMAP_PROC_CTX * proc_ctx;
} ZOMBIE_DATA;

#ifdef CONSUMED_TIME
static CMAP_CONSUMEDTIME_US consumed_time_delete = {0};
static CMAP_CONSUMEDTIME_US consumed_time_watch = {0};
#endif

/*******************************************************************************
*******************************************************************************/

static void delete_if_zombie(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc,
  CMAP_PROC_CTX * proc_ctx);

void cmap_refswatcher_add(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc,
  CMAP_PROC_CTX * proc_ctx)
{
  if(rw -> deletion) delete_if_zombie(rw, lc, proc_ctx);
  else
  {
    REF ref = {lc, cmap_util_time_us() + rw -> check_zombie_time_us}, * ref_ptr;
    if(cmap_sset_ref_add(&rw -> refs, &ref, &ref_ptr))
      cmap_lifecycle_watched(lc, CMAP_T);
    else ref_ptr -> watch_time_us = ref.watch_time_us;
  }
}

/*******************************************************************************
*******************************************************************************/

void cmap_refswatcher_rm(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc)
{
  REF ref = {lc, 0};
  cmap_sset_ref_rm_v(&rw -> refs, &ref);
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
    cmap_slist_lc_push(wrappers, &data_ -> cur);
  }
}

static REF_EXT * upd_all_ref_exts(ZOMBIE_DATA * data)
{
  REF_EXT ref_ext_tmp;
  ref_ext_tmp.lc = data -> cur;
  REF_EXT * ref_ext_cur;
  if(!cmap_sset_ref_ext_add(data -> all_ref_exts, &ref_ext_tmp, &ref_ext_cur))
    return ref_ext_cur;

  ref_ext_init(ref_ext_cur);
  CMAP_SLIST_LC_PTR * nesteds = ref_ext_cur -> nesteds;
  cmap_core_nested(data -> cur, nesteds, data -> proc_ctx);

  data -> ret = CMAP_F;
  cmap_slist_lc_ptr_apply(nesteds, upd_all_ref_exts_apply, data);

  if(data -> ret) return ref_ext_cur;
  else
  {
    if((data -> cur != data -> org) && cmap_lifecycle_is_watched(data -> cur))
    {
      cmap_refswatcher_rm(data -> rw, data -> cur);
      cmap_lifecycle_watched(data -> cur, CMAP_F);
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
  data_arg.ref_ext =
    cmap_sset_ref_ext_get(*data_ -> all_ref_exts, &ref_ext_wrapper);
  upd_way_ref_exts(&data_arg);
}

static void upd_way_ref_exts(ZOMBIE_DATA * data)
{
  REF_EXT * ref_ext = data -> ref_ext;
  if(cmap_sset_ref_ext_add(data -> way_ref_exts, ref_ext, NULL))
  {
    CMAP_SLIST_LC * wrappers = ref_ext -> wrappers;
    cmap_slist_lc_apply(wrappers, upd_way_ref_exts_apply, data);
  }
}

/*******************************************************************************
*******************************************************************************/

static void check_way_ref_exts_apply(REF_EXT * ref_ext, void * data)
{
  char * ret = data;
  if(*ret)
  {
    CMAP_LIFECYCLE * lc = ref_ext -> lc;
    CMAP_SLIST_LC * wrappers = ref_ext -> wrappers;
    int nb_refs = cmap_lifecycle_nb_refs(lc),
      nb_wrappers = cmap_slist_lc_size(wrappers);

    if(nb_refs > nb_wrappers) *ret = CMAP_F;
  }
}

static char check_way_ref_exts(CMAP_SSET_REF_EXT * way_ref_exts)
{
  char ret = CMAP_T;
  cmap_sset_ref_ext_apply(way_ref_exts, check_way_ref_exts_apply, &ret);
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
  if(cmap_sset_ref_ext_is(way_ref_exts, &ref_ext_nested)) *nested_ = NULL;
}

typedef struct
{
  CMAP_SSET_REF_EXT * way_ref_exts;
  CMAP_PROC_CTX * proc_ctx;
} DELETE_ZOMBIE_APPLY_DATA;

static void delete_zombie_apply(REF_EXT * ref_ext, void * data)
{
  DELETE_ZOMBIE_APPLY_DATA * data_ = data;

  CMAP_SLIST_LC_PTR * nesteds = ref_ext -> nesteds;
  cmap_slist_lc_ptr_apply(nesteds, delete_zombie_apply_apply,
    data_ -> way_ref_exts);

  CMAP_LIFECYCLE * lc = ref_ext -> lc;
  cmap_core_delete(lc, data_ -> proc_ctx);
}

static void delete_zombie(CMAP_SSET_REF_EXT * way_ref_exts,
  CMAP_PROC_CTX * proc_ctx)
{
  DELETE_ZOMBIE_APPLY_DATA data = {way_ref_exts, proc_ctx};
  cmap_sset_ref_ext_apply(way_ref_exts, delete_zombie_apply, &data);
}

/*******************************************************************************
*******************************************************************************/

static void delete_all_ref_exts(CMAP_SSET_REF_EXT ** all_ref_exts)
{
  while(*all_ref_exts != NULL)
    ref_ext_delete(cmap_sset_ref_ext_rm(all_ref_exts));
}

static void delete_if_zombie(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc,
  CMAP_PROC_CTX * proc_ctx)
{
#ifdef CONSUMED_TIME
  cmap_consumedtime_start(&consumed_time_delete);
#endif

  CMAP_SSET_REF_EXT * all_ref_exts = NULL, * way_ref_exts = NULL;

  ZOMBIE_DATA data = {rw, lc, lc, NULL, &all_ref_exts, &way_ref_exts, 0,
    proc_ctx};
  if(is_zombie_w_ref_exts(&data))
  {
    cmap_log_debug("[%p][%s] zombie deletion", lc, CMAP_NATURE_CHAR(lc));

    delete_zombie(way_ref_exts, proc_ctx);

    cmap_log_debug("[%p][refswatcher] %d zombie(s) deleted", rw,
      cmap_sset_ref_ext_size(way_ref_exts));
  }

  cmap_sset_ref_ext_clean(&way_ref_exts);
  delete_all_ref_exts(&all_ref_exts);

#ifdef CONSUMED_TIME
  cmap_consumedtime_stop(&consumed_time_delete);
#endif
}

/*******************************************************************************
*******************************************************************************/

static char watch_ref(CMAP_REFSWATCHER * rw, REF * ref, char deletion,
  CMAP_PROC_CTX * proc_ctx)
{
  uint64_t now = cmap_util_time_us();
  if((ref -> watch_time_us < now) || deletion)
  {
    CMAP_LIFECYCLE * lc = ref -> lc;
    cmap_lifecycle_watched(lc, CMAP_F);
    delete_if_zombie(rw, lc, proc_ctx);
    return CMAP_T;
  }
  return CMAP_F;
}

static void watch(CMAP_REFSWATCHER * rw)
{
  if(rw -> refs == NULL) return;

  cmap_log_debug("[%p][refswatcher] start watching", rw);

#ifdef CONSUMED_TIME
  cmap_consumedtime_start(&consumed_time_watch);
#endif

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(rw -> env, NULL);

  CMAP_ITERATOR_SSET_REF * it = cmap_iterator_sset_ref_create(&rw -> refs);
  while(cmap_iterator_sset_ref_has_next(it))
    if(watch_ref(rw, cmap_iterator_sset_ref_next(it), rw -> deletion, proc_ctx))
      cmap_iterator_sset_ref_rm(it);
  cmap_iterator_sset_ref_delete(it);

  cmap_proc_ctx_delete(proc_ctx, NULL);

#ifdef CONSUMED_TIME
  cmap_consumedtime_stop(&consumed_time_watch);
#endif

  cmap_log_debug("[%p][refswatcher] stop watching", rw);
}

/*******************************************************************************
*******************************************************************************/

static void watch_loop(CMAP_LOOP_TIMER * timer)
{
  watch(timer -> data);
}

static inline void loop_init(CMAP_REFSWATCHER * rw)
{
  uint64_t time_ms = rw -> check_zombie_time_us / 1000;

  rw -> timer.data = rw;
  cmap_loop_timer_start(&rw -> timer, watch_loop, time_ms, time_ms, NULL);
}

void cmap_refswatcher_stop(CMAP_REFSWATCHER * rw)
{
  cmap_loop_timer_stop(&rw -> timer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_refswatcher_delete(CMAP_REFSWATCHER * rw)
{
  rw -> deletion = CMAP_T;
  watch(rw);

  cmap_mem_free(rw);

  cmap_log_debug("[%p][refswatcher] deleted", rw);
}

CMAP_REFSWATCHER * cmap_refswatcher_create(CMAP_ENV * env)
{
  CMAP_MEM_ALLOC_PTR(rw, CMAP_REFSWATCHER);

  rw -> env = env;
  rw -> check_zombie_time_us = cmap_config_refs_check_zombie_time_us();
  rw -> refs = NULL;
  rw -> deletion = CMAP_F;

  if(cmap_kernel_state() != CMAP_KERNEL_S_EXITING) loop_init(rw);

  cmap_log_debug("[%p][refswatcher] created", rw);

  return rw;
}

/*******************************************************************************
*******************************************************************************/

#ifdef CONSUMED_TIME
void cmap_refswatcher_log_consumed_time(char lvl)
{
  cmap_consumedtime_log(lvl, &consumed_time_delete, "refswatcher/delete");
  cmap_consumedtime_log(lvl, &consumed_time_watch, "refswatcher/watch");
}
#endif
