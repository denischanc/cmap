
#include "cmap-string.h"

#include <string.h>
#include "cmap-kernel.h"
#include "cmap-common.h"

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

static void string__delete(CMAP_MAP * this)
{
  cmap_string_delete((CMAP_STRING *)this);
}

/*******************************************************************************
*******************************************************************************/

void cmap_string__append(CMAP_STRING * this, const char * val)
{
}

/*******************************************************************************
*******************************************************************************/

CMAP_STRING * cmap_string_create(const char * val)
{
  CMAP_MAP * prototype_string = cmap_kernel() -> prototype_.string_;
  CMAP_STRING * string = (CMAP_STRING *)CMAP_CALL_ARGS(prototype_string, new,
    sizeof(CMAP_STRING));
  cmap_string_init(string, val);
  return string;
}

void cmap_string_init(CMAP_STRING * string, const char * val)
{
  CMAP_MAP * super = (CMAP_MAP *)string;
  super -> nature = string__nature;
  super -> delete = string__delete;

  CMAP_KERNEL_ALLOC_PTR(internal, CMAP_INTERNAL);
  internal -> val_ = (char *)cmap_kernel() -> mem_ -> alloc(
    (strlen(val) + 1) * sizeof(char));
  strcpy(internal -> val_, val);

  string -> internal_ = internal;
  string -> append = cmap_string__append;
}

void cmap_string_delete(CMAP_STRING * string)
{
  CMAP_INTERNAL * internal = (CMAP_INTERNAL *)string -> internal_;
  CMAP_MEM * mem = cmap_kernel() -> mem_;
  CMAP_FREE(internal -> val_, mem);
  CMAP_FREE(internal, mem);

  cmap_map_delete((CMAP_MAP *)string);
}
