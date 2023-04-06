
#include "cmap-job.h"

#include "cmap-ext.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_CALL(proc_ctx, push_definitions);

  CMAP_MAP * job = cmap_proc_impl(
    "local job = function(process)/global/{"
    "  this.process = process;"
    "};"
    "job.prototype = {"
    "  schedule: function()/global/{"
    "    cmap.scheduler.addJob(this);"
    "    return this;"
    "  }"
    "}/global/;"
    "return job;", proc_ctx);

  CMAP_CALL(proc_ctx, pop_definitions);

  return job;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_JOB_PUBLIC cmap_job_public =
{
  create
};
