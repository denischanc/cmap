
#include "cmap-scheduler.h"

#include <stdlib.h>
#include <stdint.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-lifecycle.h"
#include "cmap-loop-timer.h"
#include "cmap-env.h"
#include "cmap-list.h"
#include "cmap-ptr.h"
#include "cmap-map.h"
#include "cmap-int.h"
#include "cmap-cmap.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LOOP_TIMER timer;

  CMAP_ENV * env;
  CMAP_MAP * job;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static void delete(INTERNAL * internal)
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(internal -> env);
  CMAP_DEC_REFS(internal -> job);
  cmap_proc_ctx_delete(proc_ctx, NULL);

  cmap_mem_free(internal);
}

static void delete_job(CMAP_LOOP_TIMER * timer)
{
  INTERNAL * internal = (INTERNAL *)timer;
  delete(internal);
  cmap_env_nb_jobs_add(internal -> env, -1);
}

static void delete_daemon(CMAP_LOOP_TIMER * timer)
{
  INTERNAL * internal = (INTERNAL *)timer;
  delete(internal);
  cmap_env_nb_daemons_add(internal -> env, -1);
}

/*******************************************************************************
*******************************************************************************/

static void do_proc(INTERNAL * internal, const char * name)
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(internal -> env);
  cmap_proc(internal -> job, name, proc_ctx, NULL);
  cmap_proc_ctx_delete(proc_ctx, NULL);
}

static void schedule_job(CMAP_LOOP_TIMER * timer)
{
  do_proc((INTERNAL *)timer, "run");
}

static void schedule_daemon(CMAP_LOOP_TIMER * timer)
{
  INTERNAL * internal = (INTERNAL *)timer;
  if(cmap_env_state(internal -> env) == CMAP_ENV_S_CLOSING)
  {
    do_proc(internal, "onStop");
    cmap_loop_timer_stop(timer);
  }
  else do_proc(internal, "run");
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * do_start(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args, char daemon)
{
  CMAP_ENV * env = cmap_proc_ctx_env(proc_ctx);
  if(cmap_env_state(env) == CMAP_ENV_S_CLOSING) return map;

  uint64_t timeout_ms = 0, repeat_ms = 0;

  CMAP_MAP * tmp = CMAP_LIST_SHIFT(args);
  if(tmp != NULL)
  {
    if(CMAP_NATURE(tmp) == CMAP_INT_NATURE)
      timeout_ms = cmap_int_get((CMAP_INT *)tmp);

    tmp = CMAP_LIST_SHIFT(args);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_INT_NATURE))
      repeat_ms = cmap_int_get((CMAP_INT *)tmp);
  }

  CMAP_MEM_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env;
  internal -> job = map;
  CMAP_INC_REFS(map);

  daemon = daemon && (repeat_ms > 0);
  cmap_loop_timer_start(&internal -> timer,
    daemon ? schedule_daemon : schedule_job, timeout_ms, repeat_ms,
    daemon ? delete_daemon : delete_job);

  if(daemon) cmap_env_nb_daemons_add(env, 1);
  else
  {
    cmap_env_nb_jobs_add(env, 1);

    if(repeat_ms > 0)
    {
      CMAP_PTR * internal_ = cmap_ptr_create(0, NULL, proc_ctx);
      *cmap_ptr_ref(internal_) = internal;
      CMAP_SET(map, CMAP_INTERNAL_NAME, internal_);
    }
  }

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_schedule_ms_fn(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  return do_start(proc_ctx, map, args, CMAP_F);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_daemonize_ms_fn(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  return do_start(proc_ctx, map, args, CMAP_T);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_PTR * internal = (CMAP_PTR *)CMAP_GET(map, CMAP_INTERNAL_NAME);
  if(internal != NULL)
  {
    CMAP_SET(map, CMAP_INTERNAL_NAME, NULL);

    cmap_loop_timer_stop(cmap_ptr_get(internal));
  }

  return map;
}
