
#include "cmap-prototype-int.h"

#include <stdlib.h>
#include <time.h>
#include "cmap.h"
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-int.h"
#include "cmap-string.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * value_of_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * arg = CMAP_LIST_SHIFT(args);
  if(arg != NULL)
  {
    const char * nature = CMAP_NATURE(arg);
    if(nature == CMAP_STRING_NATURE)
    {
      long v = atol(CMAP_CALL((CMAP_STRING *)arg, val));
      CMAP_CALL_ARGS((CMAP_INT *)map, set, v);
    }
    else if(nature == CMAP_INT_NATURE)
      CMAP_CALL_ARGS((CMAP_INT *)map, set, CMAP_CALL((CMAP_INT *)arg, get));
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * time_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_CALL_ARGS((CMAP_INT *)map, set, time(NULL));
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * time_us_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_CALL_ARGS((CMAP_INT *)map, set, cmap_util_time_us());

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clone_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  int64_t i = CMAP_CALL((CMAP_INT *)map, get);
  return (CMAP_MAP *)CMAP_INT(i, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_int_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_int_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "valueOf", value_of_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "time", time_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "timeUs", time_us_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "clone", clone_fn, proc_ctx);
}
