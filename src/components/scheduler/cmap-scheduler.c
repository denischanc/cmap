
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

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LOOP_TIMER timer;

  CMAP_ENV * env;
  CMAP_MAP * job;
} INTERNAL;

static const char * INTERNAL_NAME = ".cmap_internal";

/*******************************************************************************
*******************************************************************************/

static void stop(CMAP_LOOP_TIMER * timer, CMAP_ENV * env, CMAP_MAP * job)
{
  cmap_loop_timer_stop(timer);
  cmap_mem_free(timer);
  CMAP_DEC_REFS(job);
  cmap_env_nb_jobs_add(env, -1);
}

/*******************************************************************************
*******************************************************************************/

static void schedule(CMAP_LOOP_TIMER * timer)
{
  INTERNAL * internal = (INTERNAL *)timer;
  CMAP_ENV * env = internal -> env;
  CMAP_MAP * job = internal -> job;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(env);

  cmap_proc(job, "run", proc_ctx, NULL);

  if(cmap_loop_timer_is_stopped(timer)) stop(timer, env, job);

  cmap_proc_ctx_delete(proc_ctx, NULL);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_schedule_ms_fn(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  uint64_t timeout_ms = 0, repeat_ms = 0;

  CMAP_MAP * tmp = CMAP_LIST_SHIFT(args);
  if(tmp != NULL)
  {
    if(CMAP_NATURE(tmp) == CMAP_INT_NATURE)
      timeout_ms = CMAP_CALL((CMAP_INT *)tmp, get);

    tmp = CMAP_LIST_SHIFT(args);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_INT_NATURE))
      repeat_ms = CMAP_CALL((CMAP_INT *)tmp, get);
  }

  CMAP_ENV * env = cmap_proc_ctx_env(proc_ctx);

  CMAP_MEM_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env;
  internal -> job = map;
  CMAP_INC_REFS(map);
  cmap_loop_timer_start(&internal -> timer, schedule, timeout_ms, repeat_ms);

  if(repeat_ms > 0)
  {
    CMAP_PTR * internal_ = cmap_ptr_public.create(0, NULL, proc_ctx);
    *(CMAP_CALL(internal_, ref)) = internal;
    CMAP_SET(map, INTERNAL_NAME, internal_);
  }

  cmap_env_nb_jobs_add(env, 1);

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_PTR * internal_ = (CMAP_PTR *)CMAP_GET(map, INTERNAL_NAME);
  if(internal_ != NULL)
  {
    INTERNAL * internal = CMAP_CALL(internal_, get);
    CMAP_SET(map, INTERNAL_NAME, NULL);
    stop((CMAP_LOOP_TIMER *)internal, internal -> env, internal -> job);
  }

  return map;
}
