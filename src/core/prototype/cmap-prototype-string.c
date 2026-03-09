
#include "cmap-prototype-string.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-op-handler-string.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * append_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * cur;
  while((cur = CMAP_LIST_SHIFT(args, proc_ctx)) != NULL)
    cmap_op_handler_string_append((CMAP_STRING *)map, cur);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clean_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  cmap_string_clean((CMAP_STRING *)map);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clone_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  const char * val = cmap_string_val((CMAP_STRING *)map);
  return (CMAP_MAP *)CMAP_STRING(val, 0, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_string_require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_string_init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "append", append_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "clean", clean_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "clone", clone_fn, proc_ctx);
}
