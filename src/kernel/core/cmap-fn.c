
#include "cmap-fn.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_FN_NATURE = "cmap.nature.fn";

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

static void fn__delete(CMAP_MAP * this)
{
  cmap_fn_delete((CMAP_FN *)this);
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_fn__process(CMAP_FN * this, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  return internal -> process_(map, args);
}

/*******************************************************************************
*******************************************************************************/

CMAP_FN * cmap_fn_create(CMAP_FN_TPL process)
{
  CMAP_MAP * prototype_fn = cmap_kernel() -> prototype_.fn_;
  CMAP_FN * fn = (CMAP_FN *)CMAP_CALL_ARGS(prototype_fn, new,
    sizeof(CMAP_FN));
  cmap_fn_init(fn, process);
  return fn;
}

void cmap_fn_init(CMAP_FN * fn, CMAP_FN_TPL process)
{
  CMAP_MAP * super = (CMAP_MAP *)fn;
  super -> nature = fn__nature;
  super -> delete = fn__delete;

  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_ALLOC_PTR(internal, CMAP_INTERNAL, mem);
  internal -> process_ = process;

  fn -> internal_ = internal;
  fn -> process = cmap_fn__process;
}

void cmap_fn_delete(CMAP_FN * fn)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)fn -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_FREE(internal, mem);

  cmap_map_delete((CMAP_MAP *)fn);
}
