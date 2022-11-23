
#include "cmap-prototype-int.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-prototype-util.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;
static char proto_ok = CMAP_F;

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
}

static CMAP_MAP * instance(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_prototype_util_public.instance(&proto, &proto_ok, require, init,
    proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public =
{
  require,
  instance
};
