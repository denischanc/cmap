
#include "cmap-job.h"

#include <cmap/cmap.h>
#include <cmap/define-min.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_proc_impl(proc_ctx, NULL,
    "local job = function(process)/global/{"
    "  this.process = process;"
    "};"
    "job.prototype = {"
    "  schedule: function()/global/{"
    "    cmap.scheduler.addJob(this);"
    "    return this;"
    "  }"
    "}/global/;"
    "return job;");
}

/*******************************************************************************
*******************************************************************************/

const CMAP_JOB_PUBLIC cmap_job_public =
{
  create
};
