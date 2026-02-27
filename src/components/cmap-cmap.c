
#include "cmap-cmap.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-cmap-blt.h"
#include "cmap-cli.h"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_cmap_prototype(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);

  return cmap_to_map(proc_ctx,
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

static CMAP_MAP * fatal_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  cmap_lproc(cmap_get(map, "console"), "error", proc_ctx, args);
  cmap_fatal();
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_cmap_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * cmap = cmap_cmap_blt_create(proc_ctx);
  CMAP_SET(cmap, "cli", cmap_cli_create(proc_ctx));
  CMAP_SET(cmap, "fatal", cmap_fn(fatal_fn, proc_ctx));
  return cmap;
}
