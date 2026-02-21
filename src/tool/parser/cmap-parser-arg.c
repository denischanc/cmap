
#include "cmap-parser-arg.h"

#include <stdlib.h>
#include "cmap-string.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_arg_args(char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, ", %s", map);

  free(map);

  return ret;
}

char * cmap_parser_arg_args_push(char * list, char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_arg_arg_name(char * name)
{
  cmap_part_fn_arg_name(name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_arg_args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, ", \"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

char * cmap_parser_arg_args_map_push(char * list, char * name, char * map)
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

char * cmap_parser_arg_names(char * names, char * name)
{
  char * ret = NULL;
  cmap_string_append_args(&ret, "%s.%s", names, name);
  free(names);
  free(name);
  return ret;
}
