
#include "cmap-prototype-fn.h"

#include <stdlib.h>
#include "cmap-prototype-util.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;
static char proto_ok = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require()
{
  return cmap_prototype_util_public.require_map(&proto);
}

/*******************************************************************************
*******************************************************************************/

static void init()
{
}

static CMAP_MAP * instance()
{
  return cmap_prototype_util_public.instance(&proto, &proto_ok, require, init);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_FN_PUBLIC cmap_prototype_fn_public =
{
  require,
  instance
};
