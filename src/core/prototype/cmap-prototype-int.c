
#include "cmap-prototype-int.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-aisle.h"
#include "cmap-prototype-map.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * proto = NULL;

/*******************************************************************************
*******************************************************************************/

static void require()
{
  if(proto == NULL)
  {
    cmap_prototype_map_public.require();
    proto = CMAP_KERNEL_MAP();
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * instance()
{
  if(proto == NULL)
  {
    require();
  }
  return proto;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_INT_PUBLIC cmap_prototype_int_public =
{
  require,
  instance
};
