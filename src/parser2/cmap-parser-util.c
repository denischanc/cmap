
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-parser-string.h"
#include "cmap-parser-part.h"

/*******************************************************************************
*******************************************************************************/

static const char * DEFINITIONS_VAR_NAME = "definitions";
static const char * GLOBAL_ENV_VAR_NAME = "global_env";

static int id = 1;

/*******************************************************************************
*******************************************************************************/

static char * next_name()
{
  static char buffer[30];
  snprintf(buffer, sizeof(buffer), "map_%d", id++);
  return strdup(buffer);
}

/*******************************************************************************
*******************************************************************************/

static void include(char * name)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.includes(), "#include %s\n", name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

static void function_c(char * name, char is_static)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.main(),
    "%s%s %s(CMAP_PROC_CTX * proc_ctx)\n"
    "{\n"
    "  CMAP_MAP * %s = cmap_definitions(proc_ctx);\n"
    "  CMAP_MAP * %s = cmap_global_env(proc_ctx);\n\n",
    is_static ? "static " : "",
    cmap_parser_part_public.is_return() ? "CMAP_MAP *" : "void",
    name, DEFINITIONS_VAR_NAME, GLOBAL_ENV_VAR_NAME);

  if(!is_static)
  {
    cmap_parser_string_public.append_args(
      cmap_parser_part_public.definitions(),
      "%s %s(CMAP_PROC_CTX * proc_ctx);\n",
      cmap_parser_part_public.is_return() ? "CMAP_MAP *" : "void", name);
  }

  free(name);

  char * instructions = cmap_parser_part_public.pop_instructions();
  cmap_parser_string_public.append(
    cmap_parser_part_public.main(), instructions);
  free(instructions);

  cmap_parser_string_public.append(cmap_parser_part_public.main(), "}\n");
}

/*******************************************************************************
*******************************************************************************/

static char * name(char * name)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s;\n"
    "  if((%s != NULL) && cmap_is_key(%s, \"%s\"))\n"
    "    %s = cmap_get(%s, \"%s\");\n"
    "  else %s = cmap_get(%s, \"%s\");\n\n",
    map_name, DEFINITIONS_VAR_NAME, DEFINITIONS_VAR_NAME, name, map_name,
    DEFINITIONS_VAR_NAME, name, map_name, GLOBAL_ENV_VAR_NAME, name);

  free(name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s = cmap_get(%s, \"%s\");\n\n",
    map_name, map, name);

  free(map);
  free(name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void set_local(char * name, char * map)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  cmap_set(%s, \"%s\", %s);\n\n",
    DEFINITIONS_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(char * name, char * map)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  cmap_set(%s, \"%s\", %s);\n\n",
    GLOBAL_ENV_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  cmap_set(%s, \"%s\", %s);\n\n",
    src, name, map);

  free(src);
  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static char * args_push(char * list, char * map)
{
  char * ret = cmap_parser_string_public.create_args("%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = cmap_parser_string_public.create_args("\"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

static char * args_map_push(char * list, char * name, char * map)
{
  char * ret = cmap_parser_string_public.create_args("%s, \"%s\", %s",
    list, name, map);

  free(list);
  free(name);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * map_args(char * args, char * aisle)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s = cmap_to_map(proc_ctx, \"%s\"",
    map_name, aisle);
  free(aisle);

  if(args != NULL)
  {
    cmap_parser_string_public.append_args(
      cmap_parser_part_public.instructions(), ", %s", args);
    free(args);
  }

  cmap_parser_string_public.append(
    cmap_parser_part_public.instructions(), ");\n\n");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args, char * aisle)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_to_list(proc_ctx, \"%s\"",
    map_name, aisle);
  free(aisle);

  if(args != NULL)
  {
    cmap_parser_string_public.append_args(
      cmap_parser_part_public.instructions(), ", %s", args);
    free(args);
  }

  cmap_parser_string_public.append(
    cmap_parser_part_public.instructions(), ");\n\n");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string, char * aisle)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx, \"%s\");\n\n",
    map_name, string, aisle);

  free(string);
  free(aisle);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i, char * aisle)
{
  char * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_int(%s, proc_ctx, \"%s\");\n\n",
    map_name, i, aisle);

  free(i);
  free(aisle);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "  return %s;\n", map);
  free(map);

  cmap_parser_part_public.return_();
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? name(fn_name) : path(map, fn_name),
    * map_name = next_name();

  cmap_parser_string_public.append_args(
    cmap_parser_part_public.instructions(),
    "CMAP_MAP * %s = cmap_fn_proc(%s, proc_ctx, %s",
    map_name, map_fn, map_keep);
  free(map_fn);
  free(map_keep);

  if(args != NULL)
  {
    cmap_parser_string_public.append_args(
      cmap_parser_part_public.instructions(), ", %s", args);
    free(args);
  }

  cmap_parser_string_public.append(
    cmap_parser_part_public.instructions(), ");\n\n");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  include,
  function_c,
  name,
  path,
  set_local,
  set_global,
  set_path,
  args_push,
  args_map,
  args_map_push,
  map_args,
  list_args,
  string,
  int_,
  return_,
  process
};
