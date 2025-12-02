
#include "cmap-op-handler.h"

#include <stdio.h>
#include "cmap-string.h"
#include "cmap-int.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

static char match(const char * nature_l, const char * nature_r)
{
  return (nature_l == CMAP_STRING_NATURE);
}

/*******************************************************************************
*******************************************************************************/

static void op_add_self(CMAP_MAP * map_dst, CMAP_MAP * map_src)
{
  const char * nature = CMAP_NATURE(map_src);
  if(nature == CMAP_STRING_NATURE)
  {
    char * val = CMAP_CALL((CMAP_STRING *)map_src, val);
    CMAP_CALL_ARGS((CMAP_STRING *)map_dst, append, val);
  }
  else if(nature == CMAP_INT_NATURE)
  {
    char buffer[22]; /* 64 bits => 70 / 10 * 3 + 1 ("\n") = 22 */
    int64_t i = CMAP_CALL((CMAP_INT *)map_src, get);
    snprintf(buffer, sizeof(buffer), "%ld", i);
    CMAP_CALL_ARGS((CMAP_STRING *)map_dst, append, buffer);
  }
  else
  {
    char buffer[19]; /* 64 bits => 64 / 8 * 2 + 2 ("0x") + 1 ("\n") = 19 */
    snprintf(buffer, sizeof(buffer), "%p", map_src);
    CMAP_CALL_ARGS((CMAP_STRING *)map_dst, append, buffer);
  }
}

const CMAP_OP_HANDLER_SELF cmap_op_handler_string_add_self_public =
  {match, op_add_self};

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * op_add(CMAP_MAP * map_l, CMAP_MAP * map_r,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * ret = (CMAP_MAP *)CMAP_STRING("", 0, proc_ctx);
  op_add_self(ret, map_l);
  op_add_self(ret, map_r);
  return ret;
}

const CMAP_OP_HANDLER cmap_op_handler_string_add_public = {match, op_add};
