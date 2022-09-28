
#include "cmap-fn.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_FN_NATURE = "cmap.nature.fn",
  * CMAP_FN_PROTOTYPE_NAME = "prototype";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_FN_TPL process_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * fn__nature(CMAP_MAP * this)
{
  return CMAP_FN_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * fn__delete(CMAP_MAP * this)
{
  return cmap_fn_delete((CMAP_FN *)this);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn__process(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  return internal -> process_(this -> features_, map, args);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn__new(CMAP_FN * this, CMAP_LIST * args, const char * aisle)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(this, CMAP_FN_PROTOTYPE_NAME);
  if(prototype != NULL) map = CMAP_NEW_MAP(prototype, aisle);
  else map = CMAP_MAP(aisle);

  CMAP_PROCESS(this, map, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

CMAP_FN * cmap_fn_create(CMAP_FN_TPL process, const char * aisle)
{
  CMAP_MAP * prototype_fn = cmap_kernel() -> fw_.prototype_.fn_;
  CMAP_FN * fn = (CMAP_FN *)CMAP_CALL_ARGS(prototype_fn, new,
    sizeof(CMAP_FN), aisle);
  cmap_fn_init(fn, process);
  return fn;
}

void cmap_fn_init(CMAP_FN * fn, CMAP_FN_TPL process)
{
  CMAP_MAP * super = (CMAP_MAP *)fn;
  super -> nature = fn__nature;
  super -> delete = fn__delete;

  CMAP_KERNEL_ALLOC_PTR(internal, CMAP_INTERNAL);
  internal -> process_ = process;

  fn -> internal_ = internal;
  fn -> features_ = cmap_map_public.create_root(NULL);
  fn -> process = cmap_fn__process;
  fn -> new = cmap_fn__new;
}

CMAP_MAP * cmap_fn_delete(CMAP_FN * fn)
{
  CMAP_KERNEL_FREE(fn -> internal_);

  CMAP_CALL(fn -> features_, delete);

  return cmap_map_public.delete((CMAP_MAP *)fn);
}
