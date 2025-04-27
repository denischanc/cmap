
#include "cmap-scheduler.h"

#include "cmap.h"
#include "cmap-scheduler-blt.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"

/*******************************************************************************
*******************************************************************************/

static void on_schedule(uv_work_t * req, int status)
{
  CMAP_ENV * env = req -> data;

  CMAP_CALL(env, reset_scheduled);

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(env);
  cmap_scheduler_blt_public_process(proc_ctx);
  CMAP_CALL_ARGS(proc_ctx, delete, NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * schedule(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_ENV * env = CMAP_CALL(proc_ctx, env);
  CMAP_CALL(env, schedule);
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public =
{
  on_schedule,
  schedule
};
