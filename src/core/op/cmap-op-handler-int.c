
#include "cmap-op-handler.h"

#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, OP) \
static char op_##name(CMAP_MAP ** map_dst, CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if((map_l == NULL) || (map_r == NULL) || \
    (CMAP_NATURE(map_l) != CMAP_INT_NATURE) || \
    (CMAP_NATURE(map_r) != CMAP_INT_NATURE)) return (1 == 0); \
 \
  int64_t i = cmap_int_get((CMAP_INT *)map_l) OP \
    cmap_int_get((CMAP_INT *)map_r); \
  *map_dst = (CMAP_MAP *)CMAP_INT(i, proc_ctx); \
  return (1 == 1); \
} \
 \
const CMAP_OP_HANDLER cmap_op_handler_int_##name = {op_##name};

CMAP_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_IMPL(name, OP) \
static char op_##name(CMAP_MAP * map_dst, CMAP_MAP * map_src) \
{ \
  if((map_dst == NULL) || (map_src == NULL) || \
    (CMAP_NATURE(map_dst) != CMAP_INT_NATURE) || \
    (CMAP_NATURE(map_src) != CMAP_INT_NATURE)) return (1 == 0); \
 \
  int64_t i = cmap_int_get((CMAP_INT *)map_dst); \
  i OP cmap_int_get((CMAP_INT *)map_src); \
  cmap_int_set((CMAP_INT *)map_dst, i); \
  return (1 == 1); \
} \
 \
const CMAP_OP_HANDLER_SELF cmap_op_handler_int_##name = {op_##name};

CMAP_OP_SELF_LOOP(SELF_IMPL)

/*******************************************************************************
*******************************************************************************/

#define UNIQUE_IMPL(name, OP) \
static char op_##name(CMAP_MAP * map) \
{ \
  if((map == NULL) || (CMAP_NATURE(map) != CMAP_INT_NATURE)) return (1 == 0); \
 \
  int64_t i = cmap_int_get((CMAP_INT *)map); \
  i OP; \
  cmap_int_set((CMAP_INT *)map, i); \
  return (1 == 1); \
} \
 \
const CMAP_OP_HANDLER_UNIQUE cmap_op_handler_int_##name = {op_##name};

CMAP_OP_UNIQUE_LOOP(UNIQUE_IMPL)
