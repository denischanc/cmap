
#include "cmap-warehouse.h"

#include "cmap-common.h"
#include "cmap-fw.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static void warehouse__delete_aisle(CMAP_WAREHOUSE * this, const char * aisle)
{
  CMAP_MAP * map = CMAP_CALL_ARGS((CMAP_MAP *)this, get, aisle);
  while(map != NULL)
  {
    map = CMAP_CALL(map, delete);
  }

  CMAP_CALL_ARGS((CMAP_MAP *)this, set, aisle, NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * warehouse__delete(CMAP_MAP * this)
{
  CMAP_LIST * keys = CMAP_LIST(0, NULL);
  CMAP_CALL_ARGS(this, keys, keys, NULL);

  CMAP_STRING * key;
  while((key = (CMAP_STRING *)CMAP_CALL(keys, unshift)) != NULL)
  {
    warehouse__delete_aisle((CMAP_WAREHOUSE *)this, CMAP_CALL(key, val));
    CMAP_CALL((CMAP_MAP *)key, delete);
  }

  CMAP_CALL((CMAP_MAP *)keys, delete);

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
