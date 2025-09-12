
#include "cmap-parser-creator.h"

#include <stdlib.h>
#include "cmap-parser-util.h"

/*******************************************************************************
*******************************************************************************/

static char * map_args(char * args)
{
  char * map_name = NEXT_NAME("map");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(args, "%s = cmap_to_map(proc_ctx", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args)
{
  char * map_name = NEXT_NAME("list");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(args,
    "%s = (CMAP_MAP *)cmap_to_list(proc_ctx", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string)
{
  char * map_name = NEXT_NAME("string");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx);", map_name, string);
  APPEND_LF();

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i)
{
  char * map_name = NEXT_NAME("int");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_int(%s, proc_ctx);", map_name, i);
  APPEND_LF();

  free(i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * new(char * map, char * args)
{
  char * map_name = NEXT_NAME("var");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(args,
    "%s = cmap_new((CMAP_FN *)%s, proc_ctx", map_name, map);
  APPEND_LF();

  free(map);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_CREATOR_PUBLIC cmap_parser_creator_public =
  {map_args, list_args, string, int_, new};
