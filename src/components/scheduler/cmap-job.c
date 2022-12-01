
#include "cmap-job.h"

#include <cmap/cmap.h>
#include <cmap/define-min.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  return cmap$$(proc_ctx, NULL,
    "local job = $$(process)<global>{"
    "  this.process = process;"
    "};"
    "job.prototype = {"
    "  schedule: $$()<global>{"
    "    cmap.scheduler.addJob(this);"
    "    return this;"
    "  }"
    "}<global>;"
    "return job;");
}

/*******************************************************************************
*******************************************************************************/

const CMAP_JOB_PUBLIC cmap_job_public =
{
  create
};
