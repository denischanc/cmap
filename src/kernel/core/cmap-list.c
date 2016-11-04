
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
  int size_, size_inc_, size_max_, i_start_, i_stop_;
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

static CMAP_MAP * list__delete(CMAP_MAP * this)
{
  return cmap_list_delete((CMAP_LIST *)this);
}

/*******************************************************************************
*******************************************************************************/

int cmap_list__size(CMAP_LIST * this)
{
  return ((CMAP_INTERNAL *)this -> internal_) -> size_;
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
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  int size = internal -> size_;
  if((i >= 0) && (i < size))
  {
    internal -> list_[list_offset(internal, i)] = val;
  }
}

CMAP_MAP * cmap_list__get(CMAP_LIST * this, int i)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  int size = internal -> size_;
  if((i >= 0) && (i < size))
  {
    return internal -> list_[list_offset(internal, i)];
  }
  else return NULL;
}

/*******************************************************************************
*******************************************************************************/

inline void inc_i(int * i, CMAP_INTERNAL * internal)
{
  (*i)++;
  if(*i >= internal -> size_max_) *i = 0;
}

static void inc_i_stop(CMAP_INTERNAL * internal)
{
  inc_i(&internal -> i_stop_, internal);
}

static void inc_i_start(CMAP_INTERNAL * internal)
{
  inc_i(&internal -> i_start_, internal);
}

/*******************************************************************************
*******************************************************************************/

inline void dec_i(int * i, CMAP_INTERNAL * internal)
{
  (*i)--;
  if(*i < 0) *i = (internal -> size_max_ - 1);
}

static void dec_i_start(CMAP_INTERNAL * internal)
{
  dec_i(&internal -> i_start_, internal);
}

static void dec_i_stop(CMAP_INTERNAL * internal)
{
  dec_i(&internal -> i_stop_, internal);
}

/*******************************************************************************
*******************************************************************************/

static void list_mv_block(CMAP_MAP ** dst, int dst_i_start,
  CMAP_MAP ** src, int src_i_start, int src_i_stop, int src_size_max)
{
  if(src_i_start < src_i_stop)
  {
    memcpy(dst + dst_i_start, src + src_i_start,
      (src_i_stop - src_i_start) * sizeof(CMAP_MAP *));
  }
  else
  {
    list_mv_block(dst, dst_i_start, src, src_i_start, src_size_max,
      src_size_max);

    if(src_i_stop != 0)
      list_mv_block(dst, dst_i_start + (src_size_max - src_i_start),
        src, 0, src_i_stop, src_size_max);
  }
}

/*******************************************************************************
*******************************************************************************/

static void list_mv_dec(CMAP_MAP ** list, int i_start, int i_stop,
  int size_max)
{
  if(i_start < i_stop)
  {
    if(i_start == 0)
    {
      list[size_max - 1] = list[0];
      i_start = 1;
    }
    int i = i_start;
    for(; i < i_stop; i++) list[i - 1] = list[i];
  }
  else if(i_start > i_stop)
  {
    list_mv_dec(list, i_start, size_max, size_max);
    list_mv_dec(list, 0, i_stop, size_max);
  }
}

static void list_mv_inc(CMAP_MAP ** list, int i_start, int i_stop,
  int size_max)
{
  if(i_start < i_stop)
  {
    if(i_stop == size_max)
    {
      list[0] = list[size_max - 1];
      i_stop = (size_max - 1);
    }
    int i = i_stop;
    for(; i > i_start; i--) list[i] = list[i - 1];
  }
  else if(i_start > i_stop)
  {
    list_mv_inc(list, 0, i_stop, size_max);
    list_mv_inc(list, i_start, size_max, size_max);
  }
}

/*******************************************************************************
*******************************************************************************/

static void add_on_full(CMAP_INTERNAL * internal, int i, CMAP_MAP * val)
{
  CMAP_MEM * mem = cmap_kernel() -> mem_;

  int off = list_offset(internal, i), old_size_max = internal -> size_max_,
    new_size_max = old_size_max + internal -> size_inc_;
  CMAP_MAP ** new_list = (CMAP_MAP **)mem -> alloc(
    new_size_max * sizeof(CMAP_MAP *));
  CMAP_MAP ** old_list = internal -> list_;
  if(i != 0) list_mv_block(new_list, 0, old_list, internal -> i_start_, off,
    old_size_max);
  new_list[i] = val;
  if(i != old_size_max) list_mv_block(new_list, i + 1, old_list, off,
    internal -> i_stop_, old_size_max);

  internal -> size_max_ = new_size_max;
  internal -> i_start_ = 0;
  internal -> i_stop_ = old_size_max + 1;
  internal -> list_ = new_list;

  CMAP_FREE(old_list, mem);
}

/*******************************************************************************
*******************************************************************************/

static void add_on_begin(CMAP_INTERNAL * internal, CMAP_MAP * val)
{
  dec_i_start(internal);
  internal -> list_[internal -> i_start_] = val;
}

static CMAP_MAP * rm_on_begin(CMAP_INTERNAL * internal)
{
  CMAP_MAP * val = internal -> list_[internal -> i_start_];
  inc_i_start(internal);
  return val;
}

/*******************************************************************************
*******************************************************************************/

static void add_on_middle(CMAP_INTERNAL * internal, int i, CMAP_MAP * val)
{
  int off = list_offset(internal, i), i_start = internal -> i_start_,
    i_stop = internal -> i_stop_, size_max = internal -> size_max_,
    size_inc = (i_stop - off), size_dec = (off - i_start);

  if(size_inc < 0) size_inc += size_max;
  else if(size_dec < 0) size_dec += size_max;

  if(size_dec < size_inc)
  {
    list_mv_dec(internal -> list_, i_start, off, size_max);
    dec_i_start(internal);

    dec_i(&off, internal);
  }
  else
  {
    list_mv_inc(internal -> list_, off, i_stop, size_max);
    inc_i_stop(internal);
  }

  internal -> list_[off] = val;
}

static CMAP_MAP * rm_on_middle(CMAP_INTERNAL * internal, int i)
{
  int off = list_offset(internal, i), i_start = internal -> i_start_,
    i_stop = internal -> i_stop_, size_max = internal -> size_max_,
    size_dec = (i_stop - off - 1), size_inc = (off - i_start);

  CMAP_MAP * val = internal -> list_[off];

  if(size_inc < 0) size_inc += size_max;
  else if(size_dec < 0) size_dec += size_max;

  if(size_dec < size_inc)
  {
    inc_i(&off, internal);

    list_mv_dec(internal -> list_, off, i_stop, size_max);
    dec_i_stop(internal);
  }
  else
  {
    list_mv_inc(internal -> list_, i_start, off, size_max);
    inc_i_start(internal);
  }

  return val;
}

/*******************************************************************************
*******************************************************************************/

static void add_on_end(CMAP_INTERNAL * internal, CMAP_MAP * val)
{
  internal -> list_[internal -> i_stop_] = val;
  inc_i_stop(internal);
}

static CMAP_MAP * rm_on_end(CMAP_INTERNAL * internal)
{
  dec_i_stop(internal);
  return internal -> list_[internal -> i_stop_];
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__add(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  int size = internal -> size_;
  if((i >= 0) && (i <= size))
  {
    if(size == internal -> size_max_) add_on_full(internal, i, val);
    else
    {
      if(i == size) add_on_end(internal, val);
      else if(i == 0) add_on_begin(internal, val);
      else add_on_middle(internal, i, val);
    }

    internal -> size_++;
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_list__rm(CMAP_LIST * this, int i)
{
  CMAP_MAP * val = NULL;

  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  int size = internal -> size_;
  if((i >= 0) && (i < size))
  {
    if(i == (size - 1)) val = rm_on_end(internal);
    else if(i == 0) val = rm_on_begin(internal);
    else val = rm_on_middle(internal, i);

    internal -> size_--;
  }

  return val;
}

/*******************************************************************************
*******************************************************************************/

void cmap_list__push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_CALL_ARGS(this, add, CMAP_CALL(this, size), val);
}

CMAP_MAP * cmap_list__pop(CMAP_LIST * this)
{
  return CMAP_CALL_ARGS(this, rm, CMAP_CALL(this, size) - 1);
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

void cmap_list__clear(CMAP_LIST * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  internal -> size_ = 0;
  internal -> i_start_ = 0;
  internal -> i_stop_ = 0;
}

/*******************************************************************************
*******************************************************************************/

CMAP_LIST * cmap_list_create(int size_inc, const char * aisle)
{
  CMAP_MAP * prototype_list = cmap_kernel() -> fw_.prototype_.list_;
  CMAP_LIST * list = (CMAP_LIST *)CMAP_CALL_ARGS(prototype_list, new,
    sizeof(CMAP_LIST), aisle);
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
  internal -> size_ = 0;
  internal -> size_inc_ = size_inc;
  internal -> size_max_ = size_inc;
  internal -> i_start_ = 0;
  internal -> i_stop_ = 0;
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
  list -> clear = cmap_list__clear;
}

CMAP_MAP * cmap_list_delete(CMAP_LIST * list)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)list -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_FREE(internal -> list_, mem);
  CMAP_FREE(internal, mem);

  return cmap_map_delete((CMAP_MAP *)list);
}
