
#include "cmap-cmap.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-cmap-blt.h"
#include "cmap-cli.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * prototype(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);

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

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx, int argc, char ** argv)
{
  CMAP_MAP * cmap = cmap_cmap_blt_create(proc_ctx);
  CMAP_SET(cmap, "cli", cmap_cli_public.create(proc_ctx, argc, argv));
  return cmap;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMAP_PUBLIC cmap_cmap_public = { prototype, create };
