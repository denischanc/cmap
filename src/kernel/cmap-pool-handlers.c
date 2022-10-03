
#include "cmap-pool-handlers.h"

#include "cmap.h"

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

CMAP_POOL_LIST_HANDLER * cmap_pool_list_handler =
  &cmap_pool_list_handler_;

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * string_handler__create()
{
  return CMAP_STRING("", 0, NULL);
}

static void string_handler__delete(CMAP_STRING * string)
{
  CMAP_DELETE(string);
}

static void string_handler__clean(CMAP_STRING * string)
{
  CMAP_CALL(string, clear);
}

static CMAP_POOL_STRING_HANDLER cmap_pool_string_handler_ =
{
  .create = string_handler__create,
  .delete = string_handler__delete,
  .clean = string_handler__clean
};

CMAP_POOL_STRING_HANDLER * cmap_pool_string_handler =
  &cmap_pool_string_handler_;
