
#include "cmap-pool-handlers.h"

#include <stdlib.h>
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * list_create()
{
  return CMAP_LIST(0, NULL);
}

static void list_delete(CMAP_LIST * list)
{
  CMAP_DELETE(list);
}

static void list_clean(CMAP_LIST * list)
{
  CMAP_CALL(list, clear);
}

const CMAP_POOL_HANDLER_LIST_PUBLIC cmap_pool_handler_list_public =
{
  list_create,
  list_delete,
  list_clean
};

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * string_create()
{
  return CMAP_STRING("", 0, NULL);
}

static void string_delete(CMAP_STRING * string)
{
  CMAP_DELETE(string);
}

static void string_clean(CMAP_STRING * string)
{
  CMAP_CALL(string, clear);
}

const CMAP_POOL_HANDLER_STRING_PUBLIC cmap_pool_handler_string_public =
{
  string_create,
  string_delete,
  string_clean
};
