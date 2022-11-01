
#include "cmap-prototype-fn.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * instance()
{
  if(proto == NULL) proto = CMAP_KERNEL_MAP();
  return proto;
}

static CMAP_MAP * init()
{
  CMAP_MAP * proto = instance();
  return proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_FN_PUBLIC cmap_prototype_fn_public =
{
  init,
  instance
};
