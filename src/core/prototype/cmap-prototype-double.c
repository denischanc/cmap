
#include "cmap-prototype-double.h"

#include "cmap-prototype-util.h"
#include "cmap-double.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clone_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  double d = cmap_double_get((CMAP_DOUBLE *)map);
  return (CMAP_MAP *)CMAP_DOUBLE(d, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_double_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_double_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "clone", clone_fn, proc_ctx);
}
