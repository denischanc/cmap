
#include "cmap-parser-this-args.h"

#include <stdlib.h>
#include "cmap-util.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static const char * this_args[] = {"this", "args", NULL};

static const char * this_args_type[] = {"CMAP_MAP", "CMAP_LIST"};

static char * this_args_decl = NULL;

/*******************************************************************************
*******************************************************************************/

static char is(const char * map, const char * name)
{
  if(map != NULL) return (1 == 0);
  else return (cmap_util_public.static_contains(name, this_args) >= 0);
}

/*******************************************************************************
*******************************************************************************/

static const char * type(const char * name)
{
  int off = cmap_util_public.static_contains(name, this_args);
  return (off >= 0) ? this_args_type[off] : NULL;
}

/*******************************************************************************
*******************************************************************************/

static const char * decl()
{
  if(this_args_decl == NULL)
  {
    const char ** it = this_args, ** type = this_args_type;
    while(*it != NULL)
    {
      cmap_string_public.append_args(&this_args_decl, ", %s * %s", *type, *it);
      it++; type++;
    }
  }
  return this_args_decl;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_THIS_ARGS_PUBLIC cmap_parser_this_args_public =
  {is, type, decl};
