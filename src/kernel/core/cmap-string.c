
#include "cmap-string.h"

#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

#define SIZE_INC_MIN (1 << 6)
#define SIZE_INC_DFT (1 << 10)

/*******************************************************************************
*******************************************************************************/

const char * CMAP_STRING_NATURE = "cmap.nature.string";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * val_;
  int size_, size_inc_, size_max_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * string__nature(CMAP_MAP * this)
{
  return CMAP_STRING_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * string__delete(CMAP_MAP * this)
{
  return cmap_string_delete((CMAP_STRING *)this);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_string__val(CMAP_STRING * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  return internal -> val_;
}

/*******************************************************************************
*******************************************************************************/

inline int adjusted_size_max(CMAP_INTERNAL * internal, int size)
{
  int size_inc = internal -> size_inc_, new_size = (internal -> size_ + size),
    remainder = (new_size % size_inc);
  if(remainder == 0) return new_size;
  else return (new_size + size_inc - remainder);
}

/*******************************************************************************
*******************************************************************************/

static void append(CMAP_INTERNAL * internal, const char * val, int size_append)
{
  int off = internal -> size_ - 1,
    new_size_max = adjusted_size_max(internal, size_append);

  if(new_size_max > internal -> size_max_)
  {
    CMAP_MEM * mem = cmap_kernel() -> mem_;

    char * new_val = (char *)mem -> alloc(new_size_max),
      * old_val = internal -> val_;
    memcpy(new_val, old_val, off);
    mem -> free(old_val);

    internal -> val_ = new_val;
    internal -> size_max_ = new_size_max;
  }

  int new_size = (internal -> size_ + size_append);
  char * int_val = internal -> val_;
  memcpy(int_val + off, val, size_append);
  int_val[new_size - 1] = 0;

  internal -> size_ = new_size;
}

/*******************************************************************************
*******************************************************************************/

void cmap_string__append(CMAP_STRING * this, const char * val)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  append(internal, val, strlen(val));
}

/*******************************************************************************
*******************************************************************************/

void cmap_string__append_sub(CMAP_STRING * this, const char * val,
  int off_start, int off_stop)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  append(internal, val + off_start, off_stop - off_start);
}

/*******************************************************************************
*******************************************************************************/

void cmap_string__clear(CMAP_STRING * this)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)this -> internal_;
  internal -> val_[0] = 0;
  internal -> size_ = 1;
}

/*******************************************************************************
*******************************************************************************/

CMAP_STRING * cmap_string_create(const char * val, int size_inc,
  const char * aisle)
{
  CMAP_MAP * prototype_string = cmap_kernel() -> prototype_.string_;
  CMAP_STRING * string = (CMAP_STRING *)CMAP_CALL_ARGS(prototype_string, new,
    sizeof(CMAP_STRING), aisle);
  cmap_string_init(string, val, size_inc);
  return string;
}

void cmap_string_init(CMAP_STRING * string, const char * val, int size_inc)
{
  CMAP_MAP * super = (CMAP_MAP *)string;
  super -> nature = string__nature;
  super -> delete = string__delete;

  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_ALLOC_PTR(internal, CMAP_INTERNAL, mem);
  if(size_inc < SIZE_INC_MIN) size_inc = SIZE_INC_DFT;
  internal -> size_inc_ = size_inc;
  internal -> size_ = (strlen(val) + 1);
  internal -> size_max_ = adjusted_size_max(internal, 0);
  internal -> val_ = (char *)mem -> alloc(internal -> size_max_);
  memcpy(internal -> val_, val, internal -> size_);

  string -> internal_ = internal;
  string -> val = cmap_string__val;
  string -> append = cmap_string__append;
  string -> append_sub = cmap_string__append_sub;
  string -> clear = cmap_string__clear;
}

CMAP_MAP * cmap_string_delete(CMAP_STRING * string)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)string -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_FREE(internal -> val_, mem);
  CMAP_FREE(internal, mem);

  return cmap_map_delete((CMAP_MAP *)string);
}
