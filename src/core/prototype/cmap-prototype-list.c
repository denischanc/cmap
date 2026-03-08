
#include "cmap-prototype-list.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-prototype-util.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * apply_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_LIST * list = (CMAP_LIST *)map;

  CMAP_PROTOTYPE_UTIL_MAP_FN map_fn = {};
  if(cmap_prototype_util_args_to_map_fn(args, &map_fn))
  {
    CMAP_LIST * args_list_i = CMAP_LIST(0, proc_ctx);

    int size = cmap_list_size(list), i;
    for(i = 0; i < size; i++)
    {
      cmap_list_clean(args_list_i);
      CMAP_LIST_PUSH(args_list_i, CMAP_LIST_GET(list, i));
      CMAP_FN_PROC(map_fn.fn, proc_ctx, map_fn.map, args_list_i);
    }
  }

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_all_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_LIST * list = (CMAP_LIST *)map;

  CMAP_MAP * tmp = CMAP_LIST_SHIFT(args);
  if((tmp != NULL) && (CMAP_NATURE(tmp) == CMAP_LIST_NATURE))
  {
    CMAP_LIST * list2 = (CMAP_LIST *)tmp;

    int size = cmap_list_size(list2), i;
    for(i = 0; i < size; i++) CMAP_LIST_PUSH(list, CMAP_LIST_GET(list2, i));
  }

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * size_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  int size = cmap_list_size((CMAP_LIST *)map);
  return (CMAP_MAP *)CMAP_INT(size, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * push_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_LIST * list = (CMAP_LIST *)map;

  CMAP_MAP * e;
  while((e = CMAP_LIST_SHIFT(args)) != NULL) CMAP_LIST_PUSH(list, e);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * pop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  return CMAP_LIST_POP((CMAP_LIST *)map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * unshift_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_LIST * list = (CMAP_LIST *)map;

  CMAP_MAP * e;
  while((e = CMAP_LIST_SHIFT(args)) != NULL) CMAP_LIST_UNSHIFT(list, e);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * shift_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  return CMAP_LIST_SHIFT((CMAP_LIST *)map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * add_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_INT * i = (CMAP_INT *)CMAP_LIST_SHIFT(args);
  CMAP_MAP * e = CMAP_LIST_SHIFT(args);
  CMAP_LIST_ADD((CMAP_LIST *)map, cmap_int_get(i), e);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * rm_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_INT * i = (CMAP_INT *)CMAP_LIST_SHIFT(args);
  return CMAP_LIST_RM((CMAP_LIST *)map, cmap_int_get(i));
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_list_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_list_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "apply", apply_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "addAll", add_all_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "size", size_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "push", push_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "pop", pop_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "unshift", unshift_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "shift", shift_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "add", add_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "rm", rm_fn, proc_ctx);
}
