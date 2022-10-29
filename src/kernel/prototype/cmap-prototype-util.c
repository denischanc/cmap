
#include "cmap-prototype-util.h"

#include <stdlib.h>
#include <cmap/cmap.h>

/*******************************************************************************
*******************************************************************************/

const char * CMAP_MAIN_FN_NAME = "main";
const char * CMAP_APPLY_FN_NAME = "apply";
const char * CMAP_ADDALL_FN_NAME = "addAll";

/*******************************************************************************
*******************************************************************************/

char cmap_prototype_args_map_fn(CMAP_PROTOTYPE_MAP_FN * map_fn,
  CMAP_LIST * args)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_LIST_GET(args, 0);
  if(CMAP_NATURE(tmp) == cmap_fn_public.nature)
  {
    map_fn -> fn_ = (CMAP_FN *)tmp;
    return CMAP_T;
  }
  else
  {
    CMAP_MAP * map = tmp;
    tmp = CMAP_GET(map, CMAP_MAIN_FN_NAME);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == cmap_fn_public.nature))
    {
      map_fn -> map_ = map;
      map_fn -> fn_ = (CMAP_FN *)tmp;
      return CMAP_T;
    }
  }

  return CMAP_F;
}
