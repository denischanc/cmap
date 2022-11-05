
#include "cmap-prototype-int.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-util.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * init()
{
  proto = cmap_util_public.to_map(CMAP_AISLE_KERNEL,
    NULL);
  return proto;
}

static CMAP_MAP * instance()
{
  return proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public =
{
  init,
  instance
};
