
#include "cmap-aislestore.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

static void delete_aisle(CMAP_AISLESTORE * this, const char * aisle)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)CMAP_GET(this, aisle);
  while(lc != NULL) lc = CMAP_DELETE(lc);

  CMAP_SET(this, aisle, NULL);
}

/*******************************************************************************
*******************************************************************************/

static void delete_last(CMAP_AISLESTORE * this, const char * aisle)
{
  CMAP_MAP * map = CMAP_GET(this, aisle);
  if(map != NULL) CMAP_SET(this, aisle, CMAP_DELETE(map));
}

/*******************************************************************************
*******************************************************************************/

static void delete_aisle_apply_fn(const char * key, CMAP_MAP ** val,
  void * data)
{
  for(CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)*val; lc != NULL;
    lc = CMAP_DELETE(lc));

  *val = NULL;
}

static CMAP_LIFECYCLE * delete(CMAP_LIFECYCLE * as)
{
  CMAP_MAP * map = (CMAP_MAP *)as;
  CMAP_CALL_ARGS(map, apply, delete_aisle_apply_fn, NULL);
  return cmap_map_public.delete(map);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_AISLESTORE * create()
{
  CMAP_KERNEL_ALLOC_PTR(as, CMAP_AISLESTORE);
  CMAP_MAP * map = (CMAP_MAP *)as;

  cmap_map_public.init(map, NULL, NULL);
  map -> super.delete = delete;

  as -> delete = delete_aisle;
  as -> delete_last = delete_last;

  return as;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_AISLESTORE_PUBLIC cmap_aislestore_public =
{
  create
};
