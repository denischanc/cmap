
#include "cmap-pool-handler.h"

#include "cmap.h"
#include "cmap-list.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * list_ghost_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_LIST * list = CMAP_LIST(0, proc_ctx);
  CMAP_CALL((CMAP_MAP *)list, ghost);
  return list;
}

static void list_ghost_clean(CMAP_LIST * this)
{
  CMAP_CALL(this, clean);
}

const CMAP_POOL_HANDLER_LIST_GHOST_PUBLIC cmap_pool_handler_list_ghost_public =
{
  list_ghost_create,
  list_ghost_clean
};

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * map_ghost_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = CMAP_MAP(proc_ctx);
  CMAP_CALL(map, ghost);
  return map;
}

static void map_ghost_clean(CMAP_MAP * this)
{
  CMAP_CALL(this, clean);
}

const CMAP_POOL_HANDLER_MAP_GHOST_PUBLIC cmap_pool_handler_map_ghost_public =
{
  map_ghost_create,
  map_ghost_clean
};
