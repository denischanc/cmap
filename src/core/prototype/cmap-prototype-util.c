
#include "cmap-prototype-util.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static char args_to_map_fn(CMAP_LIST * args,
  CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_LIST_GET(args, 0);
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

const CMAP_PROTOTYPE_UTIL_PUBLIC cmap_prototype_util_public =
{
  args_to_map_fn
};
