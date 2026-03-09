
#include "cmap-cmap.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-cmap-blt.h"
#include "cmap-cli.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_INTERNAL_NAME = ".cmap_internal";

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_cmap_prototype(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);

  return cmap_to_map(proc_ctx,
    "map", cmap_prototypestore_map(ps, proc_ctx),
    "list", cmap_prototypestore_list(ps, proc_ctx),
    "fn", cmap_prototypestore_fn(ps, proc_ctx),
    "string", cmap_prototypestore_string(ps, proc_ctx),
    "int", cmap_prototypestore_int(ps, proc_ctx),
    "double", cmap_prototypestore_double(ps, proc_ctx),
    "ptr", cmap_prototypestore_ptr(ps, proc_ctx),
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
  CMAP_SET(cmap, "cli", cmap_cli_create(proc_ctx), proc_ctx);
  CMAP_SET(cmap, "fatal", cmap_fn(fatal_fn, proc_ctx), proc_ctx);
  return cmap;
}
