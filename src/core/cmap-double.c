
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

static const char * nature(CMAP_MAP * this)
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

static CMAP_LIFECYCLE * delete(CMAP_DOUBLE * double_)
{
  CMAP_KERNEL_FREE(double_ -> internal);

  return cmap_map_public.delete(&double_ -> super);
}

static CMAP_LIFECYCLE * delete_(CMAP_LIFECYCLE * double_)
{
  return delete((CMAP_DOUBLE *)double_);
}

static void init(CMAP_DOUBLE * double_, double val)
{
  CMAP_MAP * super = &double_ -> super;
  super -> nature = nature;
  super -> super.delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  double_ -> internal = internal;
  double_ -> get = get;
  double_ -> set = set;
}

static CMAP_DOUBLE * create(double val, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_double = CMAP_CALL_ARGS(ps, double_, proc_ctx);
  CMAP_DOUBLE * double_ =
    CMAP_PROTOTYPE_NEW(prototype_double, CMAP_DOUBLE, proc_ctx, aisle);
  init(double_, val);
  return double_;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_DOUBLE_PUBLIC cmap_double_public =
{
  create,
  init,
  delete,
  get,
  set
};
