
#include "cmap-op.h"

#include <stdlib.h>
#include "cmap-op-handler.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

#define HANDLER_IMPL(type, name) \
  if(cmap_op_handler_##type##_##name##_public.match(nature_l, nature_r)) \
    return cmap_op_handler_##type##_##name##_public.op(map_l, map_r, proc_ctx);

#define IMPL(name, OP) \
static CMAP_MAP * op_##name(CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if((map_l == NULL) || (map_r == NULL)) return NULL; \
  const char * nature_l = CMAP_NATURE(map_l), \
    * nature_r = CMAP_NATURE(map_r); \
  CMAP_OP_HANDLER_##name##_LOOP(HANDLER_IMPL) \
  return NULL; \
}

CMAP_OP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define SELF_HANDLER_IMPL(type, name) \
  if(cmap_op_handler_##type##_##name##_public.match(nature_dst, nature_src)) \
  { \
    cmap_op_handler_##type##_##name##_public.op(map_dst, map_src); \
    return; \
  }

#define SELF_IMPL(name, OP) \
static void op_##name(CMAP_MAP * map_dst, CMAP_MAP * map_src) \
{ \
  if((map_dst == NULL) || (map_src == NULL)) return; \
  const char * nature_dst = CMAP_NATURE(map_dst), \
    * nature_src = CMAP_NATURE(map_src); \
  CMAP_OP_HANDLER_##name##_LOOP(SELF_HANDLER_IMPL) \
}

CMAP_OP_SELF_LOOP(SELF_IMPL)

/*******************************************************************************
*******************************************************************************/

#define UNIQUE_HANDLER_IMPL(type, name) \
  if(cmap_op_handler_##type##_##name##_public.match(nature)) \
  { \
    cmap_op_handler_##type##_##name##_public.op(map); \
    return; \
  }

#define UNIQUE_IMPL(name, OP) \
static void op_##name(CMAP_MAP * map) \
{ \
  if(map == NULL) return; \
  const char * nature = CMAP_NATURE(map); \
  CMAP_OP_HANDLER_##name##_LOOP(UNIQUE_HANDLER_IMPL) \
}

CMAP_OP_UNIQUE_LOOP(UNIQUE_IMPL)

/*******************************************************************************
*******************************************************************************/

#define SET(name, OP) op_##name,

const CMAP_OP_PUBLIC cmap_op_public =
{
  CMAP_OP_LOOP(SET)
  CMAP_OP_SELF_LOOP(SET)
  CMAP_OP_UNIQUE_LOOP(SET)
};
