
#include "cmap-scheduler-ep.h"

#include <uv.h>
#include "cmap.h"
#include "cmap-scheduler.h"
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
  cmap_scheduler_public_process(proc_ctx);
  cmap_delete_proc_ctx(proc_ctx);
}

/* TODO : long time processing ... */

static void schedule(CMAP_ENV * env)
{
  if(!internal.scheduled)
  {
    internal.scheduled = CMAP_T;

    internal.req.data = env;
    cmap_util_public.uv_error(uv_queue_work(cmap_kernel_public.uv_loop(),
      &internal.req, cmap_util_public.uv_dummy, on_schedule));
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_scheduler_ep_public_schedule(CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  schedule(CMAP_CALL(proc_ctx, env));
  return NULL;
}
