
#include "cmap-int.h"

#include "cmap-kernel.h"
#include "cmap-prototype-int.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int64_t val;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_NATURE_INT = "cmap.nature.int";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_NATURE_INT;
}

/*******************************************************************************
*******************************************************************************/

static int64_t get(CMAP_INT * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static void set(CMAP_INT * this, int64_t val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> val = val;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_INT * int_)
{
  CMAP_KERNEL_FREE(int_ -> internal);

  return cmap_map_public.delete((CMAP_MAP *)int_);
}

static CMAP_MAP * delete_(CMAP_MAP * int_)
{
  return delete((CMAP_INT *)int_);
}

static void init(CMAP_INT * int_, int64_t val)
{
  CMAP_MAP * super = (CMAP_MAP *)int_;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  int_ -> internal = internal;
  int_ -> get = get;
  int_ -> set = set;
}

static CMAP_INT * create(int64_t val, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_MAP * prototype_int = cmap_prototype_int_public.instance(proc_ctx);
  CMAP_INT * int_ = (CMAP_INT *)CMAP_CALL_ARGS(prototype_int, new,
    sizeof(CMAP_INT), proc_ctx, aisle);
  init(int_, val);
  return int_;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_INT_PUBLIC cmap_int_public =
{
  create,
  init,
  delete,
  get,
  set
};
