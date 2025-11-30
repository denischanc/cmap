
#include "cmap-op-handler.h"

#include "cmap-double.h"

/*******************************************************************************
*******************************************************************************/

static char match(const char * nature_l, const char * nature_r)
{
  return ((nature_l == CMAP_DOUBLE_NATURE) &&
    (nature_r == CMAP_DOUBLE_NATURE));
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, OP) \
static CMAP_MAP * op_##name(CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  double d = CMAP_CALL((CMAP_DOUBLE *)map_l, get) OP \
    CMAP_CALL((CMAP_DOUBLE *)map_r, get); \
  return (CMAP_MAP *)CMAP_DOUBLE(d, proc_ctx); \
} \
 \
const CMAP_OP_HANDLER cmap_op_handler_double_##name##_public = \
  {match, op_##name};

CMAP_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_IMPL(name, OP) \
static void op_##name(CMAP_MAP * map_dst, CMAP_MAP * map_src) \
{ \
  double d = CMAP_CALL((CMAP_DOUBLE *)map_dst, get); \
  d OP CMAP_CALL((CMAP_DOUBLE *)map_src, get); \
  CMAP_CALL_ARGS((CMAP_DOUBLE *)map_dst, set, d); \
} \
 \
const CMAP_OP_HANDLER_SELF cmap_op_handler_double_##name##_public = \
  {match, op_##name};

CMAP_OP_SELF_LOOP(SELF_IMPL)
