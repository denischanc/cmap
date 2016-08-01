
#include "cmap-prototype-list.h"

#include "cmap-prototype-util.h"
#include "cmap-aisle.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == CMAP_LIST_NATURE)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_PROTOTYPE_MAP_FN map_fn = {};
    if(cmap_prototype_args_map_fn(&map_fn, args))
    {
      CMAP_LIST * args = CMAP_LIST(0, NULL);

      int size = CMAP_CALL(list, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_CALL(args, clear);
        CMAP_PUSH(args, CMAP_CALL_ARGS(list, get, i));
        CMAP_CALL_ARGS(map_fn.fn_, process, map_fn.map_, args);
      }

      CMAP_DELETE(args);
    }
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_all_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == CMAP_LIST_NATURE)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_MAP * tmp = CMAP_CALL(args, unshift);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_LIST_NATURE))
    {
      CMAP_LIST * list2 = (CMAP_LIST *)tmp;

      int size = CMAP_CALL(list2, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_CALL_ARGS(list, push, CMAP_CALL_ARGS(list2, get, i));
      }
    }
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_list_init(CMAP_MAP * proto)
{
  CMAP_SET(proto, CMAP_APPLY_FN_NAME, CMAP_FN(apply_fn, CMAP_AISLE_KERNEL));
  CMAP_SET(proto, CMAP_ADDALL_FN_NAME, CMAP_FN(add_all_fn, CMAP_AISLE_KERNEL));
}
