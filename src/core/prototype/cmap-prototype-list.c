
#include "cmap-prototype-list.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-prototype-map.h"
#include "cmap-common.h"
#include "cmap-list.h"
#include "cmap-map.h"
#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;
static char proto_ok = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == CMAP_NATURE_LIST)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_PROTOTYPE_UTIL_MAP_FN map_fn = {};
    if(cmap_prototype_util_public.args_to_map_fn(args, &map_fn))
    {
      CMAP_LIST * args_list_i = CMAP_LIST(0, NULL);

      int size = CMAP_CALL(list, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_CALL(args_list_i, clear);
        CMAP_LIST_PUSH(args_list_i, CMAP_CALL_ARGS(list, get, i));
        CMAP_FN_PROC(map_fn.fn, map_fn.map, args_list_i);
      }

      CMAP_DELETE(args_list_i);
    }
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_all_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == CMAP_NATURE_LIST)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_MAP * tmp = CMAP_CALL(args, unshift);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_NATURE_LIST))
    {
      CMAP_LIST * list2 = (CMAP_LIST *)tmp;

      int size = CMAP_CALL(list2, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_LIST_PUSH(list, CMAP_CALL_ARGS(list2, get, i));
      }
    }
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * size_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) != CMAP_NATURE_LIST) return NULL;
  else
  {
    CMAP_INT * size = CMAP_INT(NULL);
    CMAP_LIST * list = (CMAP_LIST *)map;
    CMAP_CALL_ARGS(size, set, CMAP_CALL(list, size));
    return (CMAP_MAP *)size;
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require()
{
  return cmap_prototype_util_public.require_map(&proto);
}

/*******************************************************************************
*******************************************************************************/

static void init()
{
  CMAP_PROTO_SET_FN(proto, "apply", apply_fn);
  CMAP_PROTO_SET_FN(proto, "addAll", add_all_fn);
  CMAP_PROTO_SET_FN(proto, "size", size_fn);
}

static CMAP_MAP * instance()
{
  return cmap_prototype_util_public.instance(&proto, &proto_ok, require, init);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_LIST_PUBLIC cmap_prototype_list_public =
{
  require,
  instance
};
