
#include "cmap-parser-var.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-parser-util.h"
#include "cmap-part.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = NEXT_NAME("var");
  CMAP_PART_VAR_RET ret = cmap_part_public.get_map(map, name, map_name);

  if((ret.ret.map == map_name) || !ret.ret.affected)
  {
    char * instruction = strdup("");

    if(ret.ret.map == map_name) PREPEND_MAP_VAR(map_name);
    else cmap_string_public.append_args(&instruction, "if(%s == NULL) ",
      ret.ret.map);

    const char * dst = map;
    if(dst == NULL) dst = ret.is_def ? ADD_DEFINITIONS() : ADD_GLOBAL_ENV();
    APPEND_INSTRUCTION_ARGS(
      "%s%s = cmap_get(%s, \"%s\");", instruction, ret.ret.map, dst, name);
    APPEND_LF();

    free(instruction);
  }

  free(map);
  free(name);

  return ret.ret.map;
}

static char * name(char * name)
{
  return path(NULL, name);
}

/*******************************************************************************
*******************************************************************************/

static void set_local(char * name, char * map)
{
  cmap_part_public.var_loc(name, map);

  APPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", ADD_DEFINITIONS(), name, map);
  APPEND_LF();

  free(name);
  free(map);
}

static void set_fn_arg_name(char * name, int off)
{
  cmap_part_public.var_loc(name, name);

  APPEND_VARIABLE_ARGS("CMAP_MAP * %s = cmap_list_get(args, %d);", name, off);
  PREPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", ADD_DEFINITIONS(), name, name);

  free(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  char is_def = cmap_part_public.var_no_loc(src, name, map);
  const char * dst = src;
  if(dst == NULL) dst = is_def ? ADD_DEFINITIONS() : ADD_GLOBAL_ENV();

  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", dst, name, map);
  APPEND_LF();

  free(src);
  free(name);
  free(map);
}

static void set_global(char * name, char * map)
{
  set_path(NULL, name, map);
}

/*******************************************************************************
*******************************************************************************/

static void set_sb_int(char * map, char * i, char * map_src)
{
  APPEND_INSTRUCTION_ARGS(
    "cmap_list_set((CMAP_LIST *)%s, %s, %s);", map, i, map_src);
  APPEND_LF();

  free(map);
  free(i);
  free(map_src);
}

static void set_sb_string(char * map, char * string, char * map_src)
{
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, %s, %s);", map, string, map_src);
  APPEND_LF();

  free(map);
  free(string);
  free(map_src);
}

static void set_sb_map(char * map, char * map_i, char * map_src)
{
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "cmap_list_set((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s), %s);",
    map, map_i, map_src);
  APPEND_INSTRUCTION_ARGS(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "cmap_set(%s, cmap_string_val((CMAP_STRING *)%s), %s);",
    map, map_i, map_src);
  APPEND_LF();

  free(map);
  free(map_i);
  free(map_src);
}

/*******************************************************************************
*******************************************************************************/

static char * sb_int(char * map, char * i)
{
  char * map_name = NEXT_NAME("var");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_list_get((CMAP_LIST *)%s, %s);", map_name, map, i);
  APPEND_LF();

  free(map);
  free(i);

  return map_name;
}

static char * sb_string(char * map, char * string)
{
  char * map_name = NEXT_NAME("var");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, %s);", map_name, map, string);
  APPEND_LF();

  free(map);
  free(string);

  return map_name;
}

static char * sb_map(char * map, char * map_i)
{
  char * map_name = NEXT_NAME("var");

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = cmap_list_get((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s));",
    map_name, map, map_i);
  APPEND_INSTRUCTION_ARGS(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = cmap_get(%s, cmap_string_val((CMAP_STRING *)%s));",
    map_name, map, map_i);
  APPEND_LF();

  free(map);
  free(map_i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_VAR_PUBLIC cmap_parser_var_public =
{
  path, name,
  set_local, set_fn_arg_name, set_path, set_global,
  set_sb_int, set_sb_string, set_sb_map, sb_int, sb_string, sb_map
};
