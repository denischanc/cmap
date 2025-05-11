
#include "cmap-parser-util-argnames.h"

#include <stdlib.h>
#include "cmap-strings.h"
#include "cmap-string.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

static void get_apply(const char * string, void * data)
{
  cmap_string_public.append_args((char **)data, ", \"%s\"", string);
}

static char * get()
{
  char * args = NULL;

  CMAP_STRINGS * arg_names = cmap_part_public.get_fn_arg_names();
  cmap_strings_public.apply(arg_names, get_apply, &args);
  cmap_part_public.delete_fn_arg_names();

  return args;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_ARGNAMES_PUBLIC cmap_parser_util_argnames_public =
  {get};
