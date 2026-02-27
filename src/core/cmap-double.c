
#include "cmap-double.h"

#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  double val;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_DOUBLE_NATURE = "double";

/*******************************************************************************
*******************************************************************************/

static double get(CMAP_DOUBLE * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_DOUBLE * set(CMAP_DOUBLE * this, double val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> val = val;
  return this;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  cmap_mem_free(((CMAP_DOUBLE *)this) -> internal);

  cmap_map_public.delete(this);
}

static CMAP_DOUBLE * init(CMAP_DOUBLE * this, CMAP_INITARGS * initargs,
  double val)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;

  CMAP_MEM_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  this -> internal = internal;
  this -> get = get;
  this -> set = set;

  return this;
}

static CMAP_DOUBLE * create(double val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = cmap_proc_ctx_prototypestore(proc_ctx);
  initargs.nature = CMAP_DOUBLE_NATURE;
  initargs.prototype = CMAP_CALL_ARGS(ps, double_, proc_ctx);
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;

  CMAP_MEM_ALLOC_PTR(this, CMAP_DOUBLE);
  return init(this, &initargs, val);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_DOUBLE_PUBLIC cmap_double_public =
{
  create, init, delete,
  get,
  set
};
