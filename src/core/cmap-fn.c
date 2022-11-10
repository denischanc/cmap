
#include "cmap-fn.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-prototype-fn.h"
#include "cmap-list.h"
#include "cmap-aisle.h"
#include "cmap-util.h"
#include "cmap-aislestore.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_FN_TPL process;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_NATURE_FN = "cmap.nature.fn";

const char * CMAP_PROTOTYPE_NAME = "prototype";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_NATURE_FN;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_LIST * stack_local = CMAP_LIST(0, CMAP_AISLE_STACK);

  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_MAP * ret = internal -> process(this -> features, map, args);

  cmap_util_public.delete_list_vals(stack_local);
  CMAP_AISLESTORE * as = cmap_kernel_public.aislestore();
  CMAP_CALL_ARGS(as, delete_last, CMAP_AISLE_STACK);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * new(CMAP_FN * this, CMAP_LIST * args, const char * aisle)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(this, CMAP_PROTOTYPE_NAME);
  if(prototype != NULL) map = CMAP_MAP_NEW_MAP(prototype, aisle);
  else map = CMAP_MAP(aisle);

  CMAP_STRING * aisle_string = CMAP_STRING(aisle, 0, aisle);
  CMAP_SET(map, "cmap-aisle", aisle_string);
  CMAP_FN_PROC(this, map, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_FN * fn)
{
  CMAP_KERNEL_FREE(fn -> internal);

  CMAP_CALL(fn -> features, delete);

  return cmap_map_public.delete((CMAP_MAP *)fn);
}

static CMAP_MAP * delete_(CMAP_MAP * fn)
{
  return delete((CMAP_FN *)fn);
}

static void init(CMAP_FN * fn, CMAP_FN_TPL process_)
{
  CMAP_MAP * super = (CMAP_MAP *)fn;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> process = process_;

  fn -> internal = internal;
  fn -> features = cmap_map_public.create_root(NULL);
  fn -> process = process;
  fn -> new = new;
}

static CMAP_FN * create(CMAP_FN_TPL process, const char * aisle)
{
  CMAP_MAP * prototype_fn = cmap_prototype_fn_public.instance();
  CMAP_FN * fn = (CMAP_FN *)CMAP_CALL_ARGS(prototype_fn, new,
    sizeof(CMAP_FN), aisle);
  init(fn, process);
  return fn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_PUBLIC cmap_fn_public =
{
  create,
  init,
  delete,
  process,
  new
};
