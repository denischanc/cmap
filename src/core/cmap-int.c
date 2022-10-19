
#include "cmap-int.h"

#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int64_t val;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return cmap_int_public.nature;
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

static void init(CMAP_INT * int_)
{
  CMAP_MAP * super = (CMAP_MAP *)int_;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = 0;

  int_ -> internal = internal;
  int_ -> get = get;
  int_ -> set = set;
}

static CMAP_INT * create(const char * aisle)
{
  CMAP_MAP * prototype_int = cmap_kernel_public.instance() -> prototype.int_;
  CMAP_INT * int_ = (CMAP_INT *)CMAP_CALL_ARGS(prototype_int, new,
    sizeof(CMAP_INT), aisle);
  init(int_);
  return int_;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_INT_PUBLIC cmap_int_public =
{
  "cmap.nature.int",
  get, set,
  create,
  init,
  delete
};
