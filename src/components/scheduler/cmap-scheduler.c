
#include "cmap-scheduler.h"

#include "cmap.h"
#include "cmap-cmap-blt.h"
#include "cmap-proc-ctx.h"
#include "cmap-env.h"

/*******************************************************************************
*******************************************************************************/

static void schedule(uv_idle_t * idle)
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(idle -> data);
  cmap_cmap_blt_scheduler_process(proc_ctx);
  CMAP_CALL_ARGS(proc_ctx, delete, NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * scheduler_empty(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_ENV * env = CMAP_CALL(proc_ctx, env);
  CMAP_CALL(env, scheduler_empty);
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public =
{
  schedule,
  scheduler_empty
};
