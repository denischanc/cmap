
#include "cmap-scheduler.h"

#include <cmap/cmap.h>
#define __CMAP_COMMON_H__
#include <cmap/define-min.h>
#include <stdlib.h>
#include "cmap-job.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char scheduled;

  uv_loop_t * loop;
  uv_work_t req;
} INTERNAL;

static INTERNAL internal = {CMAP_F, NULL};

/*******************************************************************************
*******************************************************************************/

static void on_schedule(uv_work_t * req, int status)
{
  internal.scheduled = CMAP_F;

  $G_$("cmap.scheduler.internal.process");
}

static void dummy(uv_work_t * req)
{
}

static void schedule(uv_loop_t * loop, CMAP_MAP * job)
{
  if(!internal.scheduled)
  {
    internal.scheduled = CMAP_T;
    internal.loop = loop;

    cmap_util_public.uv_error(
      uv_queue_work(loop, &internal.req, dummy, on_schedule));
  }

  if(job != NULL) $G_$_A("cmap.scheduler.addJob", job);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_job_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * job;
  while((job = $$(args, "unshift")) != NULL)
  {
    $K_$_A(map, "internal.jobs.push", job);
    schedule(internal.loop, NULL);
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * job = $K_$(map, "jobs.unshift");
  if(job != NULL)
  {
    $$(job, "process");
    schedule(internal.loop, NULL);
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_MAP * internal = $$MAP(CMAP_AISLE_GLOBAL,
    "jobs", $LIST(0, CMAP_AISLE_GLOBAL),
    "process", $FN(process_fn, CMAP_AISLE_GLOBAL),
    NULL);

  return $$MAP(CMAP_AISLE_GLOBAL,
    "internal", internal,
    "addJob", $FN(add_job_fn, CMAP_AISLE_GLOBAL),
    "job", cmap_job_public.create(),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_SCHEDULER_PUBLIC cmap_scheduler_public =
{
  create,
  schedule
};
