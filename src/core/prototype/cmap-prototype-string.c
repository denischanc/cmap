
#include "cmap-prototype-string.h"

#include <stdlib.h>
#include "cmap-prototype-map.h"
#include "cmap-aisle.h"
#include "cmap-common.h"

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

const CMAP_PROTOTYPE_STRING_PUBLIC cmap_prototype_string_public =
{
  require,
  instance
};
