
#include "cmap-prototype-list.h"

#include "cmap-aisle.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static const char * MAIN_FN_NAME = "main";
static const char * APPLY_FN_NAME = "apply";
static const char * ADD_ALL_FN_NAME = "addAll";

/*******************************************************************************
*******************************************************************************/

static char apply_ana_args(CMAP_LIST * args, CMAP_MAP ** map, CMAP_FN ** fn)
{
  if(CMAP_CALL(args, size) < 1) return CMAP_F;

  CMAP_MAP * tmp = CMAP_CALL(args, unshift);
  if(CMAP_NATURE(tmp) == CMAP_FN_NATURE)
  {
    *fn = (CMAP_FN *)tmp;
    return CMAP_T;
  }
  else
  {
    *map = tmp;
    tmp = CMAP_GET(tmp, MAIN_FN_NAME);
    if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_FN_NATURE))
    {
      *fn = (CMAP_FN *)tmp;
      return CMAP_T;
    }
  }

  return CMAP_F;
}

static CMAP_MAP * apply_fn(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(CMAP_NATURE(map) == CMAP_LIST_NATURE)
  {
    CMAP_LIST * list = (CMAP_LIST *)map;

    CMAP_MAP * fn_map = NULL;
    CMAP_FN * fn = NULL;
    if(apply_ana_args(args, &fn_map, &fn))
    {
      CMAP_LIST * args = CMAP_LIST(0, NULL);

      int size = CMAP_CALL(list, size), i;
      for(i = 0; i < size; i++)
      {
        CMAP_CALL(args, clear);
        CMAP_PUSH(args, CMAP_CALL_ARGS(list, get, i));
        CMAP_CALL_ARGS(fn, process, fn_map, args);
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
  /* TODO */
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_list_init(CMAP_MAP * proto)
{
  CMAP_SET(proto, APPLY_FN_NAME, CMAP_FN(apply_fn, CMAP_AISLE_KERNEL));
  CMAP_SET(proto, ADD_ALL_FN_NAME, CMAP_FN(add_all_fn, CMAP_AISLE_KERNEL));
}
