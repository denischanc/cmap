
#include "cmap-scheduler.h"

#include <cmap/cmap.h>
#define __CMAP_COMMON_H__
#include <cmap/define-min.h>
#include <stdlib.h>
#include "cmap-job.h"
#include "cmap-util.h"
#include "cmap-env.h"
#include "cmap-parser-util.h"
#include "cmap-common-super-define.h"
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

  CMAP_PROC_CTX * proc_ctx =
    cmap_proc_ctx_public.create((CMAP_ENV *)req -> data);
  cmap_parser_util_public.proc_impl(proc_ctx, NULL,
    "cmap.scheduler.internal.process();");
  CMAP_CALL(proc_ctx, delete);
}

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
  CMAP_MAP * definitions = cmap_definitions(proc_ctx);
  CMAP_SET(definitions, "this", map);

  CMAP_MAP * job = cmap$$(proc_ctx, definitions, "return this.jobs.shift();");
  if(job != NULL)
  {
    CMAP_SET(definitions, "job", job);
    cmap$$(proc_ctx, definitions, "job.process();");

    schedule(CMAP_CALL(proc_ctx, env));
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * definitions = $$MAP(proc_ctx, NULL,
    "schedule", $FN(schedule_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "process", $FN(process_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    "job", cmap_job_public.create(proc_ctx),
    NULL);

  CMAP_MAP * scheduler = cmap$$(proc_ctx, definitions,
    "local internal = {"
    "  jobs: []<global>,"
    "  process: process"
    "}<global>;"
    "local scheduler = {"
    "  internal: internal,"
    "  addJob: $$()<global>{"
    "    local job = args.shift();"
    "    this.internal.jobs.push(job);"
    "    schedule();"
    "  },"
    "  job: job"
    "}<global>;"
    "return scheduler;");

  CMAP_DELETE(definitions);

  return scheduler;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public =
{
  create
};
