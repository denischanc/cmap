
#include "cmap-list.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-kernel.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE_INC_MIN 2
#define SIZE_INC_DFT (1 << 6)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size, size_inc, size_max, i_start, i_stop;
  CMAP_MAP ** list;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_LIST_NATURE = "list";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_LIST_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static int size(CMAP_LIST * this)
{
  return ((INTERNAL *)this -> internal) -> size;
}

/*******************************************************************************
*******************************************************************************/

static int list_offset(INTERNAL * internal, int i)
{
  i += internal -> i_start;

  int size_max = internal -> size_max;
  if(i >= size_max) i -= size_max;

  return i;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * set(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  int size = internal -> size;
  if((i >= 0) && (i < size))
  {
    internal -> list[list_offset(internal, i)] = val;
  }
  return this;
}

static CMAP_MAP * get(CMAP_LIST * this, int i)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  int size = internal -> size;
  if((i >= 0) && (i < size))
  {
    return internal -> list[list_offset(internal, i)];
  }
  else return NULL;
}

/*******************************************************************************
*******************************************************************************/

inline void inc_i(int * i, INTERNAL * internal)
{
  (*i)++;
  if(*i >= internal -> size_max) *i = 0;
}

static void inc_i_stop(INTERNAL * internal)
{
  inc_i(&internal -> i_stop, internal);
}

static void inc_i_start(INTERNAL * internal)
{
  inc_i(&internal -> i_start, internal);
}

/*******************************************************************************
*******************************************************************************/

inline void dec_i(int * i, INTERNAL * internal)
{
  (*i)--;
  if(*i < 0) *i = (internal -> size_max - 1);
}

static void dec_i_start(INTERNAL * internal)
{
  dec_i(&internal -> i_start, internal);
}

static void dec_i_stop(INTERNAL * internal)
{
  dec_i(&internal -> i_stop, internal);
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

static void add_on_full(INTERNAL * internal, int i, CMAP_MAP * val)
{
  CMAP_MEM * mem = cmap_kernel_public.mem();

  int off = list_offset(internal, i), old_size_max = internal -> size_max,
    new_size_max = old_size_max + internal -> size_inc;
  CMAP_MAP ** new_list = (CMAP_MAP **)mem -> alloc(
    new_size_max * sizeof(CMAP_MAP *));
  CMAP_MAP ** old_list = internal -> list;
  if(i != 0) list_mv_block(new_list, 0, old_list, internal -> i_start, off,
    old_size_max);
  new_list[i] = val;
  if(i != old_size_max) list_mv_block(new_list, i + 1, old_list, off,
    internal -> i_stop, old_size_max);

  internal -> size_max = new_size_max;
  internal -> i_start = 0;
  internal -> i_stop = old_size_max + 1;
  internal -> list = new_list;

  CMAP_MEM_FREE(old_list, mem);
}

/*******************************************************************************
*******************************************************************************/

static void add_on_begin(INTERNAL * internal, CMAP_MAP * val)
{
  dec_i_start(internal);
  internal -> list[internal -> i_start] = val;
}

static CMAP_MAP * rm_on_begin(INTERNAL * internal)
{
  CMAP_MAP * val = internal -> list[internal -> i_start];
  inc_i_start(internal);
  return val;
}

/*******************************************************************************
*******************************************************************************/

static void add_on_middle(INTERNAL * internal, int i, CMAP_MAP * val)
{
  int off = list_offset(internal, i), i_start = internal -> i_start,
    i_stop = internal -> i_stop, size_max = internal -> size_max,
    size_inc = (i_stop - off), size_dec = (off - i_start);

  if(size_inc < 0) size_inc += size_max;
  else if(size_dec < 0) size_dec += size_max;

  if(size_dec < size_inc)
  {
    list_mv_dec(internal -> list, i_start, off, size_max);
    dec_i_start(internal);

    dec_i(&off, internal);
  }
  else
  {
    list_mv_inc(internal -> list, off, i_stop, size_max);
    inc_i_stop(internal);
  }

  internal -> list[off] = val;
}

static CMAP_MAP * rm_on_middle(INTERNAL * internal, int i)
{
  int off = list_offset(internal, i), i_start = internal -> i_start,
    i_stop = internal -> i_stop, size_max = internal -> size_max,
    size_dec = (i_stop - off - 1), size_inc = (off - i_start);

  CMAP_MAP * val = internal -> list[off];

  if(size_inc < 0) size_inc += size_max;
  else if(size_dec < 0) size_dec += size_max;

  if(size_dec < size_inc)
  {
    inc_i(&off, internal);

    list_mv_dec(internal -> list, off, i_stop, size_max);
    dec_i_stop(internal);
  }
  else
  {
    list_mv_inc(internal -> list, i_start, off, size_max);
    inc_i_start(internal);
  }

  return val;
}

/*******************************************************************************
*******************************************************************************/

static void add_on_end(INTERNAL * internal, CMAP_MAP * val)
{
  internal -> list[internal -> i_stop] = val;
  inc_i_stop(internal);
}

static CMAP_MAP * rm_on_end(INTERNAL * internal)
{
  dec_i_stop(internal);
  return internal -> list[internal -> i_stop];
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * add(CMAP_LIST * this, int i, CMAP_MAP * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  int size = internal -> size;
  if((i >= 0) && (i <= size))
  {
    if(size == internal -> size_max) add_on_full(internal, i, val);
    else
    {
      if(i == size) add_on_end(internal, val);
      else if(i == 0) add_on_begin(internal, val);
      else add_on_middle(internal, i, val);
    }

    internal -> size++;
  }
  return this;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * rm(CMAP_LIST * this, int i)
{
  CMAP_MAP * val = NULL;

  INTERNAL * internal = (INTERNAL *)this -> internal;
  int size = internal -> size;
  if((i >= 0) && (i < size))
  {
    if(i == (size - 1)) val = rm_on_end(internal);
    else if(i == 0) val = rm_on_begin(internal);
    else val = rm_on_middle(internal, i);

    internal -> size--;
  }

  return val;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * push(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_LIST_ADD(this, CMAP_CALL(this, size), val);
  return this;
}

static CMAP_MAP * pop(CMAP_LIST * this)
{
  return CMAP_LIST_RM(this, CMAP_CALL(this, size) - 1);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * unshift(CMAP_LIST * this, CMAP_MAP * val)
{
  CMAP_LIST_ADD(this, 0, val);
  return this;
}

static CMAP_MAP * shift(CMAP_LIST * this)
{
  return CMAP_LIST_RM(this, 0);
}

/*******************************************************************************
*******************************************************************************/

static void clear(CMAP_LIST * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> size = 0;
  internal -> i_start = 0;
  internal -> i_stop = 0;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIFECYCLE * delete(CMAP_LIST * list)
{
  INTERNAL * internal = (INTERNAL *)list -> internal;
  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_FREE(internal -> list, mem);
  CMAP_MEM_FREE(internal, mem);

  return cmap_map_public.delete(&list -> super);
}

static CMAP_LIFECYCLE * delete_(CMAP_LIFECYCLE * list)
{
  return delete((CMAP_LIST *)list);
}

static void init(CMAP_LIST * list, int size_inc)
{
  CMAP_MAP * super = &list -> super;
  super -> nature = nature;
  super -> super.delete = delete_;

  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem);
  if(size_inc < SIZE_INC_MIN) size_inc = SIZE_INC_DFT;
  internal -> size = 0;
  internal -> size_inc = size_inc;
  internal -> size_max = size_inc;
  internal -> i_start = 0;
  internal -> i_stop = 0;
  internal -> list = (CMAP_MAP **)mem -> alloc(size_inc * sizeof(CMAP_MAP *));

  list -> internal = internal;
  list -> size = size;
  list -> set = set;
  list -> get = get;
  list -> add = add;
  list -> rm = rm;
  list -> push = push;
  list -> pop = pop;
  list -> unshift = unshift;
  list -> shift = shift;
  list -> clear = clear;
}

static CMAP_LIST * create(int size_inc, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_list = CMAP_CALL_ARGS(ps, list_, proc_ctx);
  CMAP_LIST * list =
    CMAP_PROTOTYPE_NEW(prototype_list, CMAP_LIST, proc_ctx, aisle);
  init(list, size_inc);
  return list;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_LIST_PUBLIC cmap_list_public =
{
  create, init, delete,
  size,
  set, get,
  add,  rm,
  push, pop,
  unshift, shift,
  clear
};
