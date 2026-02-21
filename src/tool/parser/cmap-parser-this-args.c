
#include "cmap-parser-this-args.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static const char * this_args[] = {"this", "arguments", NULL},
  * maps[] = {"this", "args"},
  * decl_ = "CMAP_MAP * this, CMAP_LIST * args",
  * args_type = "CMAP_LIST *";

/*******************************************************************************
*******************************************************************************/

char cmap_parser_this_args_is(const char * map, const char * name)
{
  if(map != NULL) return (1 == 0);
  else return (cmap_util_static_contains(name, this_args) >= 0);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_parser_this_args_map(const char * name)
{
  int off = cmap_util_static_contains(name, this_args);
  return (off >= 0) ? maps[off] : NULL;
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_parser_this_args_decl()
{
  return decl_;
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_parser_this_args_args_map()
{
  return maps[1];
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_parser_this_args_type(const char * map)
{
  return strcmp(map, cmap_parser_this_args_args_map()) ? NULL : args_type;
}
