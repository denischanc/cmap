
#include "cmap-op-handler.h"

#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static char match(const char * nature_l, const char * nature_r)
{
  return ((nature_l == CMAP_STRING_NATURE) &&
    (nature_r == CMAP_STRING_NATURE));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * op_add(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_STRING * ret = CMAP_STRING("", 0, proc_ctx);
  CMAP_CALL_ARGS(ret, append, CMAP_CALL((CMAP_STRING *)map_l, val));
  CMAP_CALL_ARGS(ret, append, CMAP_CALL((CMAP_STRING *)map_r, val));
  return (CMAP_MAP *)ret;
}

const CMAP_OP_HANDLER cmap_op_handler_string_add_public = {match, op_add};

/*******************************************************************************
*******************************************************************************/

static void op_add_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  CMAP_CALL_ARGS((CMAP_STRING *)map_dst, append,
    CMAP_CALL((CMAP_STRING *)map_src, val));
}

const CMAP_OP_HANDLER_SELF cmap_op_handler_string_add_self_public =
  {match, op_add_self};
