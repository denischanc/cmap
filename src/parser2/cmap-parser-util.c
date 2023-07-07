
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

static const char * DEFINITIONS_VAR_NAME = "definitions";
static const char * GLOBAL_ENV_VAR_NAME = "global_env";

static int id = 1;

static CMAP_PARSER_STRING * include_string_ = NULL, * function_string_ = NULL,
  * main_string_ = NULL, * instructions_string_ = NULL;

/*******************************************************************************
*******************************************************************************/

static char * next_nature_name(const char * nature)
{
  char buffer[30];
  snprintf(buffer, sizeof(buffer), "%s_%d", nature, id++);
  return strdup(buffer);
}

static char * next_map_name()
{
  return next_nature_name("map");
}

static char * next_list_name()
{
  return next_nature_name("list");
}

static char * next_string_name()
{
  return next_nature_name("string");
}

static char * next_int_name()
{
  return next_nature_name("int");
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PARSER_STRING * include_string()
{
  if(include_string_ == NULL)
    include_string_ = cmap_parser_string_create("");
  return include_string_;
}

static CMAP_PARSER_STRING * function_string()
{
  if(function_string_ == NULL)
    function_string_ = cmap_parser_string_create("");
  return function_string_;
}

static CMAP_PARSER_STRING * main_string()
{
  if(main_string_ == NULL)
    main_string_ = cmap_parser_string_create("");
  return main_string_;
}

static CMAP_PARSER_STRING * instructions_string()
{
  if(instructions_string_ == NULL)
    instructions_string_ = cmap_parser_string_create("");
  return instructions_string_;
}

/*******************************************************************************
*******************************************************************************/

static void include(char * name)
{
  cmap_parser_string_append_args(include_string(), "#include %s\n", name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

static void function_c(char * name)
{
  cmap_parser_string_append_args(main_string(),
    "void %s(CMAP_PROC_CTX * proc_ctx)\n"
    "{\n"
    "  CMAP_MAP * %s = cmap_definitions(proc_ctx);\n"
    "  CMAP_MAP * %s = cmap_global_env(proc_ctx);\n\n",
    name, DEFINITIONS_VAR_NAME, GLOBAL_ENV_VAR_NAME);
  free(name);

  cmap_parser_string_append_string(main_string(), instructions_string());
  cmap_parser_string_clear(instructions_string());

  cmap_parser_string_append(main_string(), "}\n");
}

/*******************************************************************************
*******************************************************************************/

static char * name(char * name)
{
  char * map_name = next_map_name();

  cmap_parser_string_append_args(instructions_string(),
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
  char * map_name = next_map_name();

  cmap_parser_string_append_args(instructions_string(),
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
  cmap_parser_string_append_args(instructions_string(),
    "  cmap_set(%s, \"%s\", %s);\n\n",
    DEFINITIONS_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(char * name, char * map)
{
  cmap_parser_string_append_args(instructions_string(),
    "  cmap_set(%s, \"%s\", %s);\n\n",
    GLOBAL_ENV_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  cmap_parser_string_append_args(instructions_string(),
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
  CMAP_PARSER_STRING * tmp = cmap_parser_string_create_args("%s, %s",
    list, map);

  free(list);
  free(map);

  return cmap_parser_string_dupndel(tmp);
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  CMAP_PARSER_STRING * tmp = cmap_parser_string_create_args("\"%s\", %s",
    name, map);

  free(name);
  free(map);

  return cmap_parser_string_dupndel(tmp);
}

static char * args_map_push(char * list, char * name, char * map)
{
  CMAP_PARSER_STRING * tmp = cmap_parser_string_create_args("%s, \"%s\", %s",
    list, name, map);

  free(list);
  free(name);
  free(map);

  return cmap_parser_string_dupndel(tmp);
}

/*******************************************************************************
*******************************************************************************/

static char * map_args(char * args, char * aisle)
{
  char * map_name = next_map_name();

  cmap_parser_string_append_args(instructions_string(),
    "  CMAP_MAP * %s = cmap_to_map(proc_ctx, \"%s\"",
    map_name, aisle);
  free(aisle);

  if(args != NULL)
  {
    cmap_parser_string_append_args(instructions_string(), ", %s", args);
    free(args);
  }

  cmap_parser_string_append(instructions_string(), ");\n\n");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args, char * aisle)
{
  char * list_name = next_list_name();

  cmap_parser_string_append_args(instructions_string(),
    "  CMAP_LIST * %s = cmap_to_list(proc_ctx, \"%s\"",
    list_name, aisle);
  free(aisle);

  if(args != NULL)
  {
    cmap_parser_string_append_args(instructions_string(), ", %s", args);
    free(args);
  }

  cmap_parser_string_append(instructions_string(), ");\n\n");

  return list_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string, char * aisle)
{
  char * string_name = next_string_name();

  cmap_parser_string_append_args(instructions_string(),
    "  CMAP_STRING * %s = cmap_string(%s, 0, proc_ctx, \"%s\");\n\n",
    string_name, string, aisle);

  free(string);
  free(aisle);

  return string_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i, char * aisle)
{
  char * int_name = next_int_name();

  cmap_parser_string_append_args(instructions_string(),
    "  CMAP_INT * %s = cmap_int(%s, proc_ctx, \"%s\");\n\n",
    int_name, i, aisle);

  free(i);
  free(aisle);

  return int_name;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  include_string,
  function_string,
  main_string,
  instructions_string,
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
  int_
};
