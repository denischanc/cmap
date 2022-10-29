
#include "cmap-aislestore.h"

#include <stdlib.h>
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static void delete_aisle(CMAP_AISLESTORE * this, const char * aisle)
{
  CMAP_MAP * map = CMAP_GET(this, aisle);
  while(map != NULL)
  {
    map = CMAP_DELETE(map);
  }

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
  for(CMAP_MAP * map = *val; map != NULL; map = CMAP_DELETE(map));

  *val = NULL;
}

static CMAP_MAP * delete(CMAP_MAP * this)
{
  CMAP_CALL_ARGS(this, apply, delete_aisle_apply_fn, NULL);

  return cmap_map_public.delete(this);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_AISLESTORE * create()
{
  CMAP_KERNEL_ALLOC_PTR(as, CMAP_AISLESTORE);
  CMAP_MAP * map = (CMAP_MAP *)as;

  cmap_map_public.init(map);
  map -> delete = delete;

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
