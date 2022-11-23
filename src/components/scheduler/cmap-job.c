
#include "cmap-job.h"

#include <cmap/cmap.h>
#include <cmap/define-min.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * init_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  $SET(map, "process", $$(args, "shift", proc_ctx));
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * schedule_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  $G_$_A("cmap.scheduler.addJob", proc_ctx, map);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_FN * job = $FN(init_fn, proc_ctx, CMAP_AISLE_GLOBAL);
  $SET(job, CMAP_PROTOTYPE_NAME, $$MAP(proc_ctx, CMAP_AISLE_GLOBAL,
    "schedule", $FN(schedule_fn, proc_ctx, CMAP_AISLE_GLOBAL),
    NULL));
  return (CMAP_MAP *)job;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_JOB_PUBLIC cmap_job_public =
{
  create
};
