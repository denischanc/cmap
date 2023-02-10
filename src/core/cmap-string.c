
#include "cmap-string.h"

#include <string.h>
#include "cmap-kernel.h"
#include "cmap-prototype-string.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE_INC_MIN (1 << 6)
#define SIZE_INC_DFT (1 << 10)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * val;
  int size, size_inc, size_max;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_STRING_NATURE = "string";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_STRING_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static const char * val(CMAP_STRING * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static inline int adjusted_size_max(INTERNAL * internal, int size)
{
  int size_inc = internal -> size_inc, new_size = (internal -> size + size),
    remainder = (new_size % size_inc);
  if(remainder == 0) return new_size;
  else return (new_size + size_inc - remainder);
}

/*******************************************************************************
*******************************************************************************/

static void append_(INTERNAL * internal, const char * val, int size_append)
{
  int off = internal -> size - 1,
    new_size_max = adjusted_size_max(internal, size_append);

  if(new_size_max > internal -> size_max)
  {
    CMAP_MEM * mem = cmap_kernel_public.mem();

    char * new_val = (char *)mem -> alloc(new_size_max),
      * old_val = internal -> val;
    memcpy(new_val, old_val, off);
    mem -> free(old_val);

    internal -> val = new_val;
    internal -> size_max = new_size_max;
  }

  int new_size = (internal -> size + size_append);
  char * int_val = internal -> val;
  memcpy(int_val + off, val, size_append);
  int_val[new_size - 1] = 0;

  internal -> size = new_size;
}

/*******************************************************************************
*******************************************************************************/

static void append(CMAP_STRING * this, const char * val)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  append_(internal, val, strlen(val));
}

/*******************************************************************************
*******************************************************************************/

static void append_sub(CMAP_STRING * this, const char * val,
  int off_start, int off_stop)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  append_(internal, val + off_start, off_stop - off_start);
}

/*******************************************************************************
*******************************************************************************/

static void clear(CMAP_STRING * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  internal -> val[0] = 0;
  internal -> size = 1;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_STRING * string)
{
  INTERNAL * internal = (INTERNAL *)string -> internal;
  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_FREE(internal -> val, mem);
  CMAP_MEM_FREE(internal, mem);

  return cmap_map_public.delete((CMAP_MAP *)string);
}

static CMAP_MAP * delete_(CMAP_MAP * string)
{
  return delete((CMAP_STRING *)string);
}

static void init(CMAP_STRING * string, const char * val_, int size_inc)
{
  CMAP_MAP * super = &string -> super;
  super -> nature = nature;
  super -> delete = delete_;

  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem);
  if(size_inc < SIZE_INC_MIN) size_inc = SIZE_INC_DFT;
  internal -> size_inc = size_inc;
  internal -> size = (strlen(val_) + 1);
  internal -> size_max = adjusted_size_max(internal, 0);
  internal -> val = (char *)mem -> alloc(internal -> size_max);
  memcpy(internal -> val, val_, internal -> size);

  string -> internal = internal;
  string -> val = val;
  string -> append = append;
  string -> append_sub = append_sub;
  string -> clear = clear;
}

static CMAP_STRING * create(const char * val, int size_inc,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * prototype_string = cmap_prototype_string_public.instance(proc_ctx);
  CMAP_STRING * string = (CMAP_STRING *)CMAP_CALL_ARGS(prototype_string, new,
    sizeof(CMAP_STRING), proc_ctx, aisle);
  init(string, val, size_inc);
  return string;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STRING_PUBLIC cmap_string_public =
{
  create,
  init,
  delete,
  val,
  append,
  append_sub,
  clear
};
