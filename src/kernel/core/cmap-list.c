
#include "cmap-list.h"

#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE_INC_DFT (1 << 6)

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LIST_NATURE = "cmap.nature.list";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size_inc_, size_, i_start_, i_stop_;
  CMAP_MAP ** list_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * list__nature(CMAP_MAP * this)
{
  return CMAP_LIST_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static void list__delete(CMAP_MAP * this)
{
  cmap_list_delete((CMAP_LIST *)this);
}

/*******************************************************************************
*******************************************************************************/

static int list_size(CMAP_LIST * this)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__set(CMAP_LIST * this, int i, CMAP_MAP * val)
{
}

CMAP_MAP * cmap_list__get(CMAP_LIST * this, int i)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__add(CMAP_LIST * this, int i, CMAP_MAP * val)
{
}

CMAP_MAP * cmap_list__rm(CMAP_LIST * this, int i)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(this, add, list_size(this), val);
}

CMAP_MAP * cmap_list__pop(CMAP_LIST * this)
{
  return CMAP_CALL_ARGS(this, rm, list_size(this));
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__shift(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(this, add, 0, val);
}

CMAP_MAP * cmap_list__unshift(CMAP_LIST * this)
{
  return CMAP_CALL_ARGS(this, rm, 0);
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_create(int size_inc)
{
  CMAP_MAP * prototype_list = cmap_kernel() -> prototype_.list_;
  CMAP_LIST * list = (CMAP_LIST *)CMAP_CALL_ARGS(prototype_list, new,
    sizeof(CMAP_LIST));
  cmap_list_init(list, size_inc);
  return list;
}

void cmap_list_init(CMAP_LIST * list, int size_inc)
{
  CMAP_MAP * super = (CMAP_MAP *)list;
  super -> nature = list__nature;
  super -> delete = list__delete;

  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_ALLOC_PTR(internal, CMAP_INTERNAL, mem);
  if(size_inc <= 1) size_inc = SIZE_INC_DFT;
  internal -> size_inc_ = size_inc;
  internal -> size_ = size_inc;
  internal -> i_start_ = 0;
  internal -> i_stop_ = 1;
  internal -> list_ = (CMAP_MAP **)mem -> alloc(size_inc * sizeof(CMAP_MAP *));

  list -> internal_ = internal;
  list -> set = cmap_list__set;
  list -> get = cmap_list__get;
  list -> add = cmap_list__add;
  list -> rm = cmap_list__rm;
  list -> push = cmap_list__push;
  list -> pop = cmap_list__pop;
  list -> shift = cmap_list__shift;
  list -> unshift = cmap_list__unshift;
}

void cmap_list_delete(CMAP_LIST * list)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)list -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_FREE(internal -> list_, mem);
  CMAP_FREE(internal, mem);

  cmap_map_delete((CMAP_MAP *)list);
}
