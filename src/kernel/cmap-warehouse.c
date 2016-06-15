
#include "cmap-warehouse.h"

#include "cmap-fw.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static void warehouse__delete_aisle(CMAP_WAREHOUSE * this, const char * aisle)
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

static void delete_aisle_apply_fn(const char * key, CMAP_MAP ** val,
  void * data)
{
  CMAP_MAP * map = *val;
  while(map != NULL)
  {
    map = CMAP_DELETE(map);
  }

  *val = NULL;
}

static CMAP_MAP * warehouse__delete(CMAP_MAP * this)
{
  CMAP_CALL_ARGS(this, apply, delete_aisle_apply_fn, NULL);

  return cmap_map_delete(this);
}

/*******************************************************************************
*******************************************************************************/

CMAP_WAREHOUSE * cmap_warehouse_create()
{
  CMAP_KERNEL_ALLOC_PTR(wh, CMAP_WAREHOUSE);
  CMAP_MAP * map = (CMAP_MAP *)wh;

  cmap_map_init(map);
  map -> delete = warehouse__delete;

  wh -> delete = warehouse__delete_aisle;

  return wh;
}
