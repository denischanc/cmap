
#include "cmap-part-this-args.h"

#include <stdlib.h>
#include "cmap-part-var.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static const char * this_args_[] = {"this", "args", NULL};

/*******************************************************************************
*******************************************************************************/

static void add()
{
  for(int i = 0; this_args_[i] != NULL; i++)
    cmap_part_var_public.put_loc(this_args_[i], this_args_[i]);
}

/*******************************************************************************
*******************************************************************************/

static char is(const char * map, const char * name)
{
  if(map != NULL) return (1 == 0);
  else return cmap_util_public.static_contains(name, this_args_);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_THIS_ARGS_PUBLIC cmap_part_this_args_public = {add, is};
