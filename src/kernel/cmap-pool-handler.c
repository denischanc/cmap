
#include "cmap-pool-handler.h"

#include "cmap.h"
#include "cmap-list.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_pool_handler_list_ghost_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_LIST * list = CMAP_LIST(0, proc_ctx);
  CMAP_CALL((CMAP_MAP *)list, ghost);
  return list;
}

void cmap_pool_handler_list_ghost_clean(CMAP_LIST * e)
{
  CMAP_CALL(e, clean);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_pool_handler_map_ghost_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = CMAP_MAP(proc_ctx);
  CMAP_CALL(map, ghost);
  return map;
}

void cmap_pool_handler_map_ghost_clean(CMAP_MAP * e)
{
  CMAP_CALL(e, clean);
}
