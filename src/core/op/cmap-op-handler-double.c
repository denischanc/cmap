
#include "cmap-op-handler.h"

#include "cmap-double.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, OP) \
static char op_##name(CMAP_MAP ** map_dst, CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if((map_l == NULL) || (map_r == NULL) || \
    (CMAP_NATURE(map_l) != CMAP_DOUBLE_NATURE) || \
    (CMAP_NATURE(map_r) != CMAP_DOUBLE_NATURE)) return (1 == 0); \
 \
  double d = cmap_double_get((CMAP_DOUBLE *)map_l) OP \
    cmap_double_get((CMAP_DOUBLE *)map_r); \
  *map_dst = (CMAP_MAP *)CMAP_DOUBLE(d, proc_ctx); \
  return (1 == 1); \
} \
 \
const CMAP_OP_HANDLER cmap_op_handler_double_##name = {op_##name};

CMAP_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_IMPL(name, OP) \
static char op_##name(CMAP_MAP * map_dst, CMAP_MAP * map_src) \
{ \
  if((map_dst == NULL) || (map_src == NULL) || \
    (CMAP_NATURE(map_dst) != CMAP_DOUBLE_NATURE) || \
    (CMAP_NATURE(map_src) != CMAP_DOUBLE_NATURE)) return (1 == 0); \
 \
  double d = cmap_double_get((CMAP_DOUBLE *)map_dst); \
  d OP cmap_double_get((CMAP_DOUBLE *)map_src); \
  cmap_double_set((CMAP_DOUBLE *)map_dst, d); \
  return (1 == 1); \
} \
 \
const CMAP_OP_HANDLER_SELF cmap_op_handler_double_##name = {op_##name};

CMAP_OP_SELF_LOOP(SELF_IMPL)
