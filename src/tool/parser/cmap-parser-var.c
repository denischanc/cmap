
#include "cmap-parser-var.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-parser-util.h"
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-parser-this-args.h"

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_var_path(char * map, char * name)
{
  char * map_name = NEXT_NAME_VAR();
  CMAP_PART_VAR_RET ret = cmap_part_get_map(map, name, map_name);

  if(ret.ret.map == NULL) ret.ret.new = (1 == 1);
  else
  {
    free(map_name);
    map_name = ret.ret.map;
  }

  if(ret.ret.new || !ret.ret.affected)
  {
    char * instruction = strdup("");

    if(ret.ret.new) PREPEND_MAP_VAR(map_name);
    else cmap_string_append_args(&instruction, "if(%s == NULL) ", map_name);

    const char * dst = map;
    if(dst == NULL) dst = ret.is_def ? ADD_DEFINITIONS() : ADD_GLOBAL_ENV();
    APPEND_INSTRUCTION_ARGS(
      "%s%s = cmap_get(%s, \"%s\");", instruction, map_name, dst, name);
    APPEND_LF();

    free(instruction);
  }

  free(map);
  free(name);

  return map_name;
}

char * cmap_parser_var_name(char * name)
{
  return cmap_parser_var_path(NULL, name);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_var_set_local(char * name, char * map)
{
  char * map_name = NEXT_NAME_VAR();
  cmap_part_loc(name, map_name);

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("%s = %s;", map_name, map);
  APPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", ADD_DEFINITIONS(), name, map_name);
  APPEND_LF();

  free(name);
  free(map);
  free(map_name);
}

char * cmap_parser_var_set_fn_arg_name(const char * name, int off)
{
  char * map_name = NEXT_NAME_VAR();
  cmap_part_loc(name, map_name);

  APPEND_VARIABLE_ARGS("CMAP_MAP * %s = cmap_list_get(%s, %d);",
    map_name, cmap_parser_this_args_args_map(), off);
  PREPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", ADD_DEFINITIONS(), name, map_name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_var_set_path(char * src, char * name, char * map)
{
  char * map_name = NEXT_NAME_VAR(),
    is_def = cmap_part_no_loc(src, name, map_name);

  const char * dst = src;
  if(dst == NULL) dst = is_def ? ADD_DEFINITIONS() : ADD_GLOBAL_ENV();

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("%s = %s;", map_name, map);
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", dst, name, map_name);
  APPEND_LF();

  free(src);
  free(name);
  free(map);
  free(map_name);
}

void cmap_parser_var_set_global(char * name, char * map)
{
  cmap_parser_var_set_path(NULL, name, map);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_var_set_sb_int(char * map, char * i, char * map_src)
{
  APPEND_INSTRUCTION_ARGS(
    "cmap_list_set((CMAP_LIST *)%s, %s, %s);", map, i, map_src);
  APPEND_LF();

  free(map);
  free(i);
  free(map_src);
}

void cmap_parser_var_set_sb_string(char * map, char * string, char * map_src)
{
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", map, string, map_src);
  APPEND_LF();

  free(map);
  free(string);
  free(map_src);
}

void cmap_parser_var_set_sb_map(char * map, char * map_i, char * map_src)
{
  APPEND_INSTRUCTION_ARGS("cmap_set_w_map(%s, %s, %s);", map, map_i, map_src);
  APPEND_LF();

  free(map);
  free(map_i);
  free(map_src);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_var_sb_int(char * map, char * i)
{
  char * map_name = NEXT_NAME_VAR();

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_list_get((CMAP_LIST *)%s, %s);", map_name, map, i);
  APPEND_LF();

  free(map);
  free(i);

  return map_name;
}

char * cmap_parser_var_sb_string(char * map, char * string)
{
  char * map_name = NEXT_NAME_VAR();

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, \"%s\");", map_name, map, string);
  APPEND_LF();

  free(map);
  free(string);

  return map_name;
}

char * cmap_parser_var_sb_map(char * map, char * map_i)
{
  char * map_name = NEXT_NAME_VAR();

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get_w_map(%s, %s);", map_name, map, map_i);
  APPEND_LF();

  free(map);
  free(map_i);

  return map_name;
}
