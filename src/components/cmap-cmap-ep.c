
#include "cmap-cmap-ep.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-aisle-ext.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_cmap_ep_public_prototype(CMAP_PROC_CTX * proc_ctx)
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
