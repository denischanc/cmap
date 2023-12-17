
#include "cmap-prototype-string.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-prototype-util.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * append_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if((map != NULL) && (args != NULL) &&
    (CMAP_NATURE(map) == CMAP_STRING_NATURE))
  {
    CMAP_MAP * cur;
    while((cur = CMAP_LIST_SHIFT(args)) != NULL)
    {
      const char * cur_nature = CMAP_NATURE(cur);
      if(cur_nature == CMAP_STRING_NATURE)
      {
        char * cur_val = CMAP_CALL((CMAP_STRING *)cur, val);
        CMAP_CALL_ARGS((CMAP_STRING *)map, append, cur_val);
      }
      else if(cur_nature == CMAP_INT_NATURE)
      {
        char buffer[22]; /* 64 bits => 70 / 10 * 3 + 1 ("\n") = 22 */
        int64_t cur_i = CMAP_CALL((CMAP_INT *)cur, get);
        snprintf(buffer, sizeof(buffer), "%ld", cur_i);
        CMAP_CALL_ARGS((CMAP_STRING *)map, append, buffer);
      }
      else
      {
        char buffer[19]; /* 64 bits => 64 / 8 * 2 + 2 ("0x") + 1 ("\n") = 19 */
        snprintf(buffer, sizeof(buffer), "%p", cur);
        CMAP_CALL_ARGS((CMAP_STRING *)map, append, buffer);
      }
    }
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * clean_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if((map != NULL) && (CMAP_NATURE(map) == CMAP_STRING_NATURE))
    CMAP_CALL((CMAP_STRING *)map, clean);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static void require(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx)
{
  cmap_prototype_util_public.require_map(proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_MAP * proto, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "append", append_fn, proc_ctx);
  CMAP_PROTO_SET_FN(proto, "clean", clean_fn, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_STRING_PUBLIC cmap_prototype_string_public =
{
  require,
  init
};
