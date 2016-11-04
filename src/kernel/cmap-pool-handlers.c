
#include "cmap-pool-handlers.h"

#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * list_handler__create()
{
  return CMAP_LIST(0, NULL);
}

static void list_handler__delete(CMAP_LIST * list)
{
  CMAP_DELETE(list);
}

static void list_handler__clean(CMAP_LIST * list)
{
  CMAP_CALL(list, clear);
}

static CMAP_POOL_LIST_HANDLER cmap_pool_list_handler_ =
{
  .create = list_handler__create,
  .delete = list_handler__delete,
  .clean = list_handler__clean
};

CMAP_POOL_LIST_HANDLER * cmap_pool_list_handler = &cmap_pool_list_handler_;
