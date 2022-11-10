
#include "cmap-prototype-util.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-aisle.h"
#include "cmap-prototype-map.h"

/*******************************************************************************
*******************************************************************************/

static char args_to_map_fn(CMAP_LIST * args,
  CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_LIST_UNSHIFT(args);
  if(CMAP_NATURE(tmp) == CMAP_NATURE_FN)
  {
    map_fn -> fn = (CMAP_FN *)tmp;
    return CMAP_T;
  }
  else
  {
    CMAP_MAP * map = tmp;
    tmp = CMAP_GET(map, "main");
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_NATURE_FN))
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

static CMAP_MAP * require_map(CMAP_MAP ** proto)
{
  if(*proto == NULL)
  {
    CMAP_MAP * proto_map = cmap_prototype_map_public.require();
    *proto = CMAP_MAP_NEW_MAP(proto_map, CMAP_AISLE_KERNEL);
    cmap_prototype_map_public.instance();
  }
  return *proto;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * instance(CMAP_MAP ** proto, char * ok,
  CMAP_MAP * (*require)(), void (*init)())
{
  if(!*ok)
  {
    *ok = CMAP_T;
    require();
    init();
  }
  return *proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_UTIL_PUBLIC cmap_prototype_util_public =
{
  args_to_map_fn,
  require_map,
  instance
};
