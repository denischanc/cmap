
#include "cmap-part-this-args.h"

#include <string.h>
#include "cmap-strings.h"
#include "cmap-part-var.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * this_args_ = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * this_args()
{
  if(this_args_ == NULL)
  {
    cmap_strings_public.add(&this_args_, "this");
    cmap_strings_public.add(&this_args_, "args");
  }
  return this_args_;
}

/*******************************************************************************
*******************************************************************************/

static void add_apply(const char * string, void * data)
{
  cmap_part_var_public.put_loc(string, string);
}

static void add()
{
  cmap_strings_public.apply(this_args(), add_apply, NULL);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char ret;
  const char * name;
} IS_APPLY_DATA;

static void is_apply(const char * string, void * data)
{
  IS_APPLY_DATA * data_ = data;
  if(!data_ -> ret) data_ -> ret = !strcmp(string, data_ -> name);
}

static char is(const char * map, const char * name)
{
  if(map != NULL) return (1 == 0);

  IS_APPLY_DATA data = {(1 == 0), name};
  cmap_strings_public.apply(this_args(), is_apply, &data);
  return data.ret;
}

/*******************************************************************************
*******************************************************************************/

static void clean()
{
  cmap_strings_public.delete(&this_args_);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_THIS_ARGS_PUBLIC cmap_part_this_args_public = {add, is, clean};
