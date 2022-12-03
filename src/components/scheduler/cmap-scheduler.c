
#include "cmap-scheduler.h"

#include <uv.h>
#include "cmap.h"
#include "cmap-util.h"
#include "cmap-kernel.h"
#include "cmap-aisle-ext.h"
#include "cmap-proc-ctx.h"
#include "cmap-job.h"

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
  cmap_proc_impl(proc_ctx, NULL, "cmap.scheduler.internal.process();");
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

static CMAP_MAP * schedule_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  schedule(CMAP_CALL(proc_ctx, env));
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * definitions = cmap_require_definitions(proc_ctx);
  cmap_set(definitions, "this", map);

  CMAP_MAP * job = cmap_proc_impl(proc_ctx, definitions,
    "return this.jobs.shift();");
  if(job != NULL)
  {
    cmap_set(definitions, "job", job);
    cmap_proc_impl(proc_ctx, definitions, "job.process();");

    schedule(CMAP_CALL(proc_ctx, env));
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

/* TODO : while((local job = args.shift()) != null) ... */

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * definitions = cmap_to_map(proc_ctx, NULL,
    "schedule", cmap_fn(schedule_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "process", cmap_fn(process_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "job", cmap_job_public.create(proc_ctx),
    NULL);

  CMAP_MAP * scheduler = cmap_proc_impl(proc_ctx, definitions,
    "local internal = {"
    "  jobs: []/global/,"
    "  process: process"
    "}/global/;"
    "local scheduler = {"
    "  internal: internal,"
    "  addJob: function()/global/{"
    "    local job = args.shift();"
    "    this.internal.jobs.push(job);"
    "    schedule();"
    "  },"
    "  job: job"
    "}/global/;"
    "return scheduler;");

  cmap_delete(definitions);

  return scheduler;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public =
{
  create
};
