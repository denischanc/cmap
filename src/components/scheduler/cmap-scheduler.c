
#include "cmap-scheduler.h"

#include <stdlib.h>
#include <stdint.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-proc-ctx.h"
#include "cmap-lifecycle.h"
#include "cmap-uv.h"
#include "cmap-env.h"
#include "cmap-list.h"
#include "cmap-ptr.h"
#include "cmap-map.h"
#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  uv_timer_t timer;

  CMAP_ENV * env;
  CMAP_MAP * job;

  uint64_t repeat_ms;
  uint64_t next_ts_ms;
} SCHEDULE_DATA;

static const char * DATA_NAME = "cmap_data";

/*******************************************************************************
*******************************************************************************/

static void stop(uv_timer_t * timer, CMAP_ENV * env, CMAP_MAP * job)
{
  cmap_uv_timer_stop(timer, CMAP_T);
  CMAP_DEC_REFS(job);
  CMAP_CALL_ARGS(env, nb_jobs_add, -1);
}

/*******************************************************************************
*******************************************************************************/

static void schedule(uv_timer_t * timer)
{
  SCHEDULE_DATA * data = (SCHEDULE_DATA *)timer;
  CMAP_ENV * env = data -> env;
  CMAP_MAP * job = data -> job;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(env);

  cmap_proc(job, "process", proc_ctx, NULL);

  if(data -> repeat_ms == 0) stop(timer, env, job);

  CMAP_CALL_ARGS(proc_ctx, delete, NULL);

  if(data -> repeat_ms > 0)
  {
    while(data -> next_ts_ms <= cmap_uv_now_ms())
      data -> next_ts_ms += data -> repeat_ms;
    uv_timer_set_repeat(timer, data -> next_ts_ms - cmap_uv_now_ms());
    uv_timer_again(timer);
  }
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

  CMAP_ENV * env = CMAP_CALL(proc_ctx, env);

  CMAP_MEM_INSTANCE_ALLOC_PTR(data, SCHEDULE_DATA);
  data -> env = env;
  data -> job = map;
  CMAP_INC_REFS(map);
  data -> repeat_ms = repeat_ms;
  if(repeat_ms > 0) data -> next_ts_ms = cmap_uv_now_ms() + timeout_ms;
  cmap_uv_timer_start(&data -> timer, schedule, timeout_ms, repeat_ms);

  if(repeat_ms > 0)
  {
    CMAP_PTR * data_ = cmap_ptr_public.create(0, NULL, proc_ctx);
    *(CMAP_CALL(data_, ref)) = data;
    CMAP_SET(map, DATA_NAME, data_);
  }

  CMAP_CALL_ARGS(env, nb_jobs_add, 1);

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_PTR * data_ = (CMAP_PTR *)CMAP_GET(map, DATA_NAME);
  if(data_ != NULL)
  {
    SCHEDULE_DATA * data = CMAP_CALL(data_, get);
    CMAP_SET(map, DATA_NAME, NULL);
    stop((uv_timer_t *)data, data -> env, data -> job);
  }

  return map;
}
