
#include "cmap-prototype-string.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;
static char proto_ok = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * append_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if((map != NULL) && (args != NULL) &&
    (CMAP_NATURE(map) == CMAP_STRING_NATURE))
  {
    CMAP_MAP * tmp;
    while((tmp = CMAP_LIST_SHIFT(args)) != NULL)
    {
      if(CMAP_NATURE(tmp) == CMAP_STRING_NATURE)
      {
        CMAP_STRING * append = (CMAP_STRING *)tmp;
        CMAP_STRING * string = (CMAP_STRING *)map;
        CMAP_CALL_ARGS(string, append, CMAP_CALL(append, val));
      }
    }
  }
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_prototype_util_public.require_map(&proto, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTO_SET_FN(proto, "append", append_fn, proc_ctx);
}

static CMAP_MAP * instance(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_prototype_util_public.instance(&proto, &proto_ok, require, init,
    proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_STRING_PUBLIC cmap_prototype_string_public =
{
  require,
  instance
};
