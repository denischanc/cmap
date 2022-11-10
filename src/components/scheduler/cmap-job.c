
#include "cmap-job.h"

#include <cmap/cmap.h>
#include <cmap/define-min.h>
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * init_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  $SET(map, "process", $$(args, "unshift"));
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * schedule_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  $G_$_A("cmap.scheduler.addJob", map);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_FN * job = $FN(init_fn, CMAP_AISLE_GLOBAL);
  $SET(job, CMAP_PROTOTYPE_NAME, $$MAP(CMAP_AISLE_GLOBAL,
    "schedule", $FN(schedule_fn, CMAP_AISLE_GLOBAL),
    NULL));
  return (CMAP_MAP *)job;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_JOB_PUBLIC cmap_job_public =
{
  create
};
