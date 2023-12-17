
#include "cmap-scheduler.h"

#include <uv.h>
#include "cmap.h"
#include "cmap-scheduler-blt.h"
#include "cmap-util.h"
#include "cmap-proc-ctx.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char scheduled;

  uv_work_t req;
} INTERNAL;

static INTERNAL internal = {CMAP_F, {}};

/*******************************************************************************
*******************************************************************************/

static void on_schedule(uv_work_t * req, int status)
{
  internal.scheduled = CMAP_F;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx((CMAP_ENV *)req -> data);
  cmap_scheduler_blt_public_process(proc_ctx);
  cmap_delete_proc_ctx(proc_ctx, NULL);
}

static void do_schedule(CMAP_ENV * env)
{
  if(!internal.scheduled)
  {
    internal.scheduled = CMAP_T;

    internal.req.data = env;
    cmap_util_public.uv_error(uv_queue_work(CMAP_KERNEL_INSTANCE -> uv_loop(),
      &internal.req, cmap_util_public.uv_dummy, on_schedule));
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * schedule(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  do_schedule(CMAP_CALL(proc_ctx, env));
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public = { schedule };
