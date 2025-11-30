
#include "cmap-op-handler.h"

#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static char match(const char * nature_l, const char * nature_r)
{
  return ((nature_l == CMAP_INT_NATURE) && (nature_r == CMAP_INT_NATURE));
}

static char unique_match(const char * nature)
{
  return (nature == CMAP_INT_NATURE);
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, OP) \
static CMAP_MAP * op_##name(CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  int64_t i = CMAP_CALL((CMAP_INT *)map_l, get) OP \
    CMAP_CALL((CMAP_INT *)map_r, get); \
  return (CMAP_MAP *)CMAP_INT(i, proc_ctx); \
} \
 \
const CMAP_OP_HANDLER cmap_op_handler_int_##name##_public = {match, op_##name};

CMAP_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_IMPL(name, OP) \
static void op_##name(CMAP_MAP * map_dst, CMAP_MAP * map_src) \
{ \
  int64_t i = CMAP_CALL((CMAP_INT *)map_dst, get); \
  i OP CMAP_CALL((CMAP_INT *)map_src, get); \
  CMAP_CALL_ARGS((CMAP_INT *)map_dst, set, i); \
} \
 \
const CMAP_OP_HANDLER_SELF cmap_op_handler_int_##name##_public = \
  {match, op_##name};

CMAP_OP_SELF_LOOP(SELF_IMPL)

/*******************************************************************************
*******************************************************************************/

#define UNIQUE_IMPL(name, OP) \
static void op_##name(CMAP_MAP * map) \
{ \
  int64_t i = CMAP_CALL((CMAP_INT *)map, get); \
  i OP; \
  CMAP_CALL_ARGS((CMAP_INT *)map, set, i); \
} \
 \
const CMAP_OP_HANDLER_UNIQUE cmap_op_handler_int_##name##_public = \
  {unique_match, op_##name};

CMAP_OP_UNIQUE_LOOP(UNIQUE_IMPL)
