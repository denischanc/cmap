
#include "cmap-prototype-int.h"

#include <stdlib.h>
#include <time.h>
#include "cmap.h"
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-map.h"
#include "cmap-int.h"
#include "cmap-string.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static void require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_public.require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

#define OP_FN(name, op) \
static CMAP_MAP * name##_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map, \
  CMAP_LIST * args) \
{ \
  CMAP_MAP * arg = CMAP_LIST_SHIFT(args); \
  if(CMAP_NATURE(arg) == CMAP_INT_NATURE) \
    return (CMAP_MAP *)CMAP_CALL_ARGS((CMAP_INT *)map, name, \
      CMAP_CALL((CMAP_INT *)arg, get)); \
  else return map; \
}

CMAP_INT_OP_LOOP(OP_FN)

/*******************************************************************************
*******************************************************************************/

#define STEP_FN(name, op) \
static CMAP_MAP * name##_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map, \
  CMAP_LIST * args) \
{ \
  return (CMAP_MAP *)CMAP_CALL((CMAP_INT *)map, name); \
}

CMAP_INT_STEP_LOOP(STEP_FN)

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * value_of_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * arg = CMAP_LIST_SHIFT(args);
  if((arg != NULL) && (CMAP_NATURE(arg) == CMAP_STRING_NATURE))
  {
    long v = atol(CMAP_CALL((CMAP_STRING *)arg, val));
    CMAP_CALL_ARGS((CMAP_INT *)map, set, v);
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
  CMAP_CALL_ARGS((CMAP_INT *)map, set, cmap_util_public.time_us());

  return map;
}

/*******************************************************************************
*******************************************************************************/

#define OP_STEP_INIT_FN(name, op) \
  CMAP_PROTO_SET_FN(proto, #name, name##_fn, proc_ctx);

static void init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INT_OP_LOOP(OP_STEP_INIT_FN)
  CMAP_INT_STEP_LOOP(OP_STEP_INIT_FN)

  CMAP_PROTO_SET_FN(proto, "valueOf", value_of_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "time", time_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "timeUs", time_us_fn, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public =
{
  require,
  init
};
