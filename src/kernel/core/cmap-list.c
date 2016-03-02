
#include "cmap-list.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE_INC_MIN 2
#define SIZE_INC_DFT (1 << 6)

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LIST_NATURE = "cmap.nature.list";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size_inc_, size_max_, i_start_, i_stop_;
  char full_;
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

int cmap_list__size(CMAP_LIST * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  int i_start = internal -> i_start_, i_stop = internal -> i_stop_;
  if(i_start == i_stop)
  {
    if(internal -> full_) return internal -> size_max_;
    else return 0;
  }
  else
  {
    if(i_start < i_stop) return i_stop - i_start;
    else return (internal -> size_max_ - (i_start - i_stop));
  }
}

/*******************************************************************************
*******************************************************************************/

static int list_offset(CMAP_INTERNAL * internal, int i)
{
  i += internal -> i_start_;

  int size_max = internal -> size_max_;
  if(i >= size_max) i -= size_max;

  return i;
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__set(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  int size = CMAP_CALL(this, size);
  if((i > 0) && (i < size))
  {
    CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
    internal -> list_[list_offset(internal, i)] = val;
  }
}

CMAP_MAP * cmap_list__get(CMAP_LIST * this, int i)
{
  int size = CMAP_CALL(this, size);
  if((i > 0) && (i < size))
  {
    CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
    return internal -> list_[list_offset(internal, i)];
  }
  else return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void inc_i_stop(CMAP_INTERNAL * internal)
{
  internal -> i_stop_++;

  int size_max = internal -> size_max_;
  if(internal -> i_stop_ >= size_max) internal -> i_stop_ -= size_max;

  if(internal -> i_start_ == internal -> i_stop_) internal -> full_ = CMAP_T;
}

static void dec_i_start(CMAP_INTERNAL * internal)
{
  internal -> i_start_--;

  int size_max = internal -> size_max_;
  if(internal -> i_start_ < 0) internal -> i_start_ += size_max;

  if(internal -> i_start_ == internal -> i_stop_) internal -> full_ = CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

static void list_mv(CMAP_MAP ** dst, int dst_i_start,
  CMAP_MAP ** src, int src_i_start, int src_i_stop, int size_max)
{
  if(src_i_start <= src_i_stop)
  {
    memcpy(dst + dst_i_start, src + src_i_start,
      (src_i_stop - src_i_start) * sizeof(CMAP_MAP *));
  }
  else
  {
    list_mv(dst, dst_i_start, src, src_i_start, size_max, size_max);
    list_mv(dst, dst_i_start + (size_max - src_i_start), src, 0, src_i_stop,
      size_max);
  }
}

/*******************************************************************************
*******************************************************************************/

static void add_on_full(CMAP_INTERNAL * internal, int i, CMAP_MAP * val)
{
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  int off = list_offset(internal, i);

  internal -> size_max_ += internal -> size_inc_;
  CMAP_MAP ** new_list = (CMAP_MAP **)mem -> alloc(
    internal -> size_max_ * sizeof(CMAP_MAP *));

  CMAP_MAP ** old_list = internal -> list_;
  int size_max = internal -> size_max_;
  list_mv(new_list, 0, old_list, internal -> i_start_, off, size_max);
  new_list[i] = val;
  list_mv(new_list, off + 1, old_list, off, internal -> i_stop_, size_max);

  CMAP_FREE(internal -> list_, mem);
  internal -> list_ = new_list;
}

static void add_on_begin(CMAP_INTERNAL * internal, CMAP_MAP * val)
{
  dec_i_start(internal);
  internal -> list_[internal -> i_start_] = val;
}

static void add_on_middle(CMAP_INTERNAL * internal, int i, CMAP_MAP * val)
{
}

static void add_on_end(CMAP_INTERNAL * internal, CMAP_MAP * val)
{
  internal -> list_[internal -> i_stop_] = val;
  inc_i_stop(internal);
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__add(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  if(i >= 0)
  {
    CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
    if(internal -> i_start_ == internal -> i_stop_)
    {
      if(!internal -> full_)
      {
        if(i == 0) add_on_end(internal, val);
      }
      else
      {
        if(i <= internal -> size_max_) add_on_full(internal, i, val);
      }
    }
    else
    {
      int size = CMAP_CALL(this, size);
      if(i == 0) add_on_begin(internal, val);
      else if(i < size) add_on_middle(internal, i, val);
      else if(i == size) add_on_end(internal, val);
    }
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_list__rm(CMAP_LIST * this, int i)
{
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(this, add, CMAP_CALL(this, size), val);
}

CMAP_MAP * cmap_list__pop(CMAP_LIST * this)
{
  return CMAP_CALL_ARGS(this, rm, CMAP_CALL(this, size));
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
  if(size_inc < SIZE_INC_MIN) size_inc = SIZE_INC_DFT;
  internal -> size_inc_ = size_inc;
  internal -> size_max_ = size_inc;
  internal -> i_start_ = 0;
  internal -> i_stop_ = 0;
  internal -> full_ = CMAP_F;
  internal -> list_ = (CMAP_MAP **)mem -> alloc(size_inc * sizeof(CMAP_MAP *));

  list -> internal_ = internal;
  list -> size = cmap_list__size;
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
