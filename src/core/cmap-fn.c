
#include "cmap-fn.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include <cmap/cmap.h>

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_FN_TPL process;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return cmap_fn_public.nature;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> process(this -> features, map, args);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * new(CMAP_FN * this, CMAP_LIST * args, const char * aisle)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(this, cmap_fn_public.prototype_name);
  if(prototype != NULL) map = CMAP_MAP_NEW_MAP(prototype, aisle);
  else map = CMAP_MAP(aisle);

  CMAP_FN_PROCESS(this, map, args);

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
  CMAP_MAP * prototype_fn = cmap_kernel_public.instance() -> prototype.fn_;
  CMAP_FN * fn = (CMAP_FN *)CMAP_CALL_ARGS(prototype_fn, new,
    sizeof(CMAP_FN), aisle);
  init(fn, process);
  return fn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_PUBLIC cmap_fn_public =
{
  "cmap.nature.fn",
  "prototype",
  create,
  init,
  delete,
  process,
  new
};
