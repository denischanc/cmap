
#include "cmap-prototype-list.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-common.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == cmap_list_public.nature)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_PROTOTYPE_MAP_FN map_fn = {};
    if(cmap_prototype_args_map_fn(&map_fn, args))
    {
      CMAP_LIST * args_list_i = CMAP_LIST(0, NULL);

      int size = CMAP_CALL(list, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_CALL(args_list_i, clear);
        CMAP_LIST_PUSH(args_list_i, CMAP_CALL_ARGS(list, get, i));
        CMAP_FN_PROCESS(map_fn.fn_, map_fn.map_, args_list_i);
      }

      CMAP_DELETE(args_list_i);
    }
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_all_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == cmap_list_public.nature)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_MAP * tmp = CMAP_CALL(args, unshift);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == cmap_list_public.nature))
    {
      CMAP_LIST * list2 = (CMAP_LIST *)tmp;

      int size = CMAP_CALL(list2, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_LIST_PUSH(list, CMAP_CALL_ARGS(list2, get, i));
      }
    }
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * init()
{
  proto = CMAP_KERNEL_MAP();
  CMAP_PROTO_SET_FN(proto, "apply", apply_fn);
  CMAP_PROTO_SET_FN(proto, "addAll", add_all_fn);
  return proto;
}

static CMAP_MAP * instance()
{
  return proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_LIST_PUBLIC cmap_prototype_list_public =
{
  init,
  instance
};
