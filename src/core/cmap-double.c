
#include "cmap-double.h"

#include "cmap-kernel.h"
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

static const char * nature(CMAP_LIFECYCLE * this)
{
  return CMAP_DOUBLE_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static double get(CMAP_DOUBLE * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static void set(CMAP_DOUBLE * this, double val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> val = val;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  CMAP_KERNEL_FREE(((CMAP_DOUBLE *)this) -> internal);

  cmap_map_public.delete(this);
}

static void init(CMAP_DOUBLE * this, double val)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nature = nature;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  this -> internal = internal;
  this -> get = get;
  this -> set = set;
}

static CMAP_DOUBLE * create(double val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_double = CMAP_CALL_ARGS(ps, double_, proc_ctx);
  CMAP_DOUBLE * this =
    CMAP_PROTOTYPE_NEW(prototype_double, CMAP_DOUBLE, proc_ctx);
  init(this, val);
  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_DOUBLE_PUBLIC cmap_double_public =
{
  create, init, delete,
  get,
  set
};
