
#include "cmap-int.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_INT_NATURE = "cmap.nature.int";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int64_t val_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * int__nature(CMAP_MAP * this)
{
  return CMAP_INT_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * int__delete(CMAP_MAP * this)
{
  return cmap_int_delete((CMAP_INT *)this);
}

/*******************************************************************************
*******************************************************************************/

int64_t cmap_int__get(CMAP_INT * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  return internal -> val_;
}

/*******************************************************************************
*******************************************************************************/

void cmap_int__set(CMAP_INT * this, int64_t val)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  internal -> val_ = val;
}

/*******************************************************************************
*******************************************************************************/

CMAP_INT * cmap_int_create(const char * aisle)
{
  CMAP_MAP * prototype_int = cmap_kernel() -> prototype_.int_;
  CMAP_INT * _int = (CMAP_INT *)CMAP_CALL_ARGS(prototype_int, new,
    sizeof(CMAP_INT), aisle);
  cmap_int_init(_int);
  return _int;
}

void cmap_int_init(CMAP_INT * _int)
{
  CMAP_MAP * super = (CMAP_MAP *)_int;
  super -> nature = int__nature;
  super -> delete = int__delete;

  CMAP_KERNEL_ALLOC_PTR(internal, CMAP_INTERNAL);
  internal -> val_ = 0;

  _int -> internal_ = internal;
  _int -> get = cmap_int__get;
  _int -> set = cmap_int__set;
}

CMAP_MAP * cmap_int_delete(CMAP_INT * _int)
{
  CMAP_KERNEL_FREE(_int -> internal_);

  return cmap_map_delete((CMAP_MAP *)_int);
}
