
#include "cmap-cmap.h"

#include "cmap.h"
#include "cmap-ext.h"
#include "cmap-aisle-ext.h"
#include "cmap-console.h"
#include "cmap-scheduler.h"
#include <stdlib.h>
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create_prototype(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);

  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL,
    "map", CMAP_CALL_ARGS(ps, map_, proc_ctx),
    "list", CMAP_CALL_ARGS(ps, list_, proc_ctx),
    "fn", CMAP_CALL_ARGS(ps, fn_, proc_ctx),
    "string", CMAP_CALL_ARGS(ps, string_, proc_ctx),
    "int", CMAP_CALL_ARGS(ps, int_, proc_ctx),
    "double", CMAP_CALL_ARGS(ps, double_, proc_ctx),
    "ptr", CMAP_CALL_ARGS(ps, ptr_, proc_ctx),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL,
    "prototype", create_prototype(proc_ctx),
    "console", cmap_console_public_create(proc_ctx),
    "scheduler", cmap_scheduler_public.create(proc_ctx),
    NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMAP_PUBLIC cmap_cmap_public =
{
  create
};
