
#include "cmap-parser-arg.h"

#include <stdlib.h>
#include "cmap-string.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

static char * args(char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, ", %s", map);

  free(map);

  return ret;
}

static char * args_push(char * list, char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void arg_name(char * name)
{
  cmap_part_public.ctx.fn_arg_name(name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, ", \"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

static char * args_map_push(char * list, char * name, char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s, \"%s\", %s", list, name, map);

  free(list);
  free(name);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * names(char * names, char * name)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s.%s", names, name);
  free(names);
  free(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_ARG_PUBLIC cmap_parser_arg_public =
{
  args, args_push,
  arg_name,
  args_map, args_map_push,
  names
};
