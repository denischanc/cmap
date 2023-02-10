
#include "cmap-double.h"

#include "cmap-kernel.h"
#include "cmap-prototype-double.h"

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

static CMAP_MAP * delete(CMAP_DOUBLE * double_)
{
  CMAP_KERNEL_FREE(double_ -> internal);

  return cmap_map_public.delete((CMAP_MAP *)double_);
}

static CMAP_MAP * delete_(CMAP_MAP * double_)
{
  return delete((CMAP_DOUBLE *)double_);
}

static void init(CMAP_DOUBLE * double_, double val)
{
  CMAP_MAP * super = &double_ -> super;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  double_ -> internal = internal;
  double_ -> get = get;
  double_ -> set = set;
}

static CMAP_DOUBLE * create(double val, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_MAP * prototype_double = cmap_prototype_double_public.instance(proc_ctx);
  CMAP_DOUBLE * double_ = (CMAP_DOUBLE *)CMAP_CALL_ARGS(prototype_double, new,
    sizeof(CMAP_DOUBLE), proc_ctx, aisle);
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
