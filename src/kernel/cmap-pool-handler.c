
#include "cmap-pool-handler.h"

#include "cmap.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * list_create(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_LIST(0, proc_ctx);
}

static void list_clean(CMAP_LIST * this)
{
  CMAP_CALL(this, clean);
}

const CMAP_POOL_HANDLER_LIST_PUBLIC cmap_pool_handler_list_public =
{
  list_create,
  list_clean
};

/*******************************************************************************
*******************************************************************************/

static CMAP_STRING * string_create(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_STRING("", 0, proc_ctx);
}

static void string_clean(CMAP_STRING * this)
{
  CMAP_CALL(this, clean);
}

const CMAP_POOL_HANDLER_STRING_PUBLIC cmap_pool_handler_string_public =
{
  string_create,
  string_clean
};

/*******************************************************************************
*******************************************************************************/

static CMAP_INT * int_create(CMAP_PROC_CTX * proc_ctx)
{
  return CMAP_INT(0, proc_ctx);
}

static void int_clean(CMAP_INT * this)
{
  CMAP_CALL_ARGS(this, set, 0);
}

const CMAP_POOL_HANDLER_INT_PUBLIC cmap_pool_handler_int_public =
{
  int_create,
  int_clean
};
