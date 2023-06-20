
#include "cmap-prototype-util.h"

#include "cmap.h"
#include "cmap-list.h"
#include "cmap-map.h"
#include "cmap-fn.h"
#include "cmap-aisle.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

static char args_to_map_fn(CMAP_LIST * args,
  CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_LIST_SHIFT(args);
  if(CMAP_NATURE(tmp) == CMAP_FN_NATURE)
  {
    map_fn -> fn = (CMAP_FN *)tmp;
    return CMAP_T;
  }
  else
  {
    CMAP_MAP * map = tmp;
    tmp = CMAP_GET(map, "main");
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_FN_NATURE))
    {
      map_fn -> map = map;
      map_fn -> fn = (CMAP_FN *)tmp;
      return CMAP_T;
    }
  }

  return CMAP_F;
}

/*******************************************************************************
*******************************************************************************/

static void require_map(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * proto_map = CMAP_CALL_ARGS(ps, require_map, proc_ctx);
  *proto = CMAP_PROTOTYPE_NEW(proto_map, CMAP_MAP, proc_ctx, CMAP_AISLE_GLOBAL);
  CMAP_CALL_ARGS(ps, map_, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_UTIL_PUBLIC cmap_prototype_util_public =
{
  args_to_map_fn,
  require_map
};
