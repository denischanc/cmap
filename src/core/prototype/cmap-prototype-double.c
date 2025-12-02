
#include "cmap-prototype-double.h"

#include "cmap-prototype-util.h"
#include "cmap-double.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clone_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  double d = CMAP_CALL((CMAP_DOUBLE *)map, get);
  return (CMAP_MAP *)CMAP_DOUBLE(d, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_public.require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "clone", clone_fn, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_DOUBLE_PUBLIC cmap_prototype_double_public =
{
  require,
  init
};
