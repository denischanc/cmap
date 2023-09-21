
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-part.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS(part, txt, args...) \
  cmap_parser_string_public.append_args( \
    cmap_parser_part_public.part(), txt, args)

#define APPEND(part, txt) \
  cmap_parser_string_public.append( \
    cmap_parser_part_public.part(), txt)

/*******************************************************************************
*******************************************************************************/

static const char * DEFINITIONS_VAR_NAME = "definitions";
static const char * GLOBAL_ENV_VAR_NAME = "global_env";
static char * def_n_global_env_decl = NULL;

static int id = 1;

/*******************************************************************************
*******************************************************************************/

static char * next_name()
{
  static char buffer[30];
  snprintf(buffer, sizeof(buffer), "cmap_gen_name__%d", id++);
  return strdup(buffer);
}

/*******************************************************************************
*******************************************************************************/

static char * def_n_global_env_decl_()
{
  if(def_n_global_env_decl == NULL)
  {
    def_n_global_env_decl = strdup("");
    cmap_parser_string_public.append_args(&def_n_global_env_decl,
    "  CMAP_MAP * %s = cmap_definitions(proc_ctx);\n"
    "  CMAP_MAP * %s = cmap_global_env(proc_ctx);\n\n",
    DEFINITIONS_VAR_NAME, GLOBAL_ENV_VAR_NAME);
  }
  return def_n_global_env_decl;
}

/*******************************************************************************
*******************************************************************************/

static void add_args(char * args)
{
  if(args != NULL)
  {
    APPEND_ARGS(instructions, ", %s", args);
    free(args);
  }

  APPEND(instructions, ", NULL);\n\n");
}

static void add_aisle(char * aisle)
{
  if(aisle != NULL)
  {
    APPEND_ARGS(instructions, "\"%s\"", aisle);
    free(aisle);
  }
  else APPEND(instructions, "NULL");
}

/*******************************************************************************
*******************************************************************************/

static void include(char * name)
{
  APPEND_ARGS(includes, "#include %s\n", name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

static void function_c(char * name, char is_static)
{
  APPEND_ARGS(main, "%s%s %s(CMAP_PROC_CTX * proc_ctx)\n{\n%s",
    is_static ? "static " : "",
    cmap_parser_part_public.is_return() ? "CMAP_MAP *" : "void",
    name, def_n_global_env_decl_());

  if(!is_static)
  {
    APPEND_ARGS(definitions,
      "%s %s(CMAP_PROC_CTX * proc_ctx);\n",
      cmap_parser_part_public.is_return() ? "CMAP_MAP *" : "void", name);
  }

  free(name);

  char * instructions = cmap_parser_part_public.pop_instructions();
  APPEND(main, instructions);
  free(instructions);

  APPEND(main, "}\n\n");
}

/*******************************************************************************
*******************************************************************************/

static char * name(char * name)
{
  char * map_name = next_name();

  APPEND_ARGS(instructions,
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

  APPEND_ARGS(instructions,
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
  APPEND_ARGS(instructions,
    "  cmap_set(%s, \"%s\", %s);\n\n",
    DEFINITIONS_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(char * name, char * map)
{
  APPEND_ARGS(instructions,
    "  cmap_set(%s, \"%s\", %s);\n\n",
    GLOBAL_ENV_VAR_NAME, name, map);

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  APPEND_ARGS(instructions,
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
  char * ret = NULL;
  cmap_parser_string_public.append_args(&ret, "%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * arg_names(char * name)
{
  char * ret = NULL;
  cmap_parser_string_public.append_args(&ret, "\"%s\"", name);

  free(name);

  return ret;
}

static char * arg_names_push(char * list, char * name)
{
  char * ret = NULL;
  cmap_parser_string_public.append_args(&ret, "%s, \"%s\"", list, name);

  free(list);
  free(name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_parser_string_public.append_args(&ret, "\"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

static char * args_map_push(char * list, char * name, char * map)
{
  char * ret = NULL;
  cmap_parser_string_public.append_args(&ret,
    "%s, \"%s\", %s", list, name, map);

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

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = cmap_to_map(proc_ctx, ", map_name);
  add_aisle(aisle);
  add_args(args);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args, char * aisle)
{
  char * map_name = next_name();

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_to_list(proc_ctx, ", map_name);
  add_aisle(aisle);
  add_args(args);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string, char * aisle)
{
  char * map_name = next_name();

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx, ",
    map_name, string);
  add_aisle(aisle);
  APPEND(instructions, ");\n\n");

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i, char * aisle)
{
  char * map_name = next_name();

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_int(%s, proc_ctx, ",
    map_name, i);
  add_aisle(aisle);
  APPEND(instructions, ");\n\n");

  free(i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  APPEND_ARGS(instructions, "  return %s;\n", map);
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

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s",
    map_name, map_fn, map_keep);
  free(map_fn);
  free(map_keep);

  add_args(args);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args)
{
  char * map_name = next_name();

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL",
    map_name, fn);
  free(fn);

  add_args(args);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  if(need_ret)
  {
    char * map_name = next_name();

    APPEND_ARGS(instructions, "  CMAP_MAP * %s = %s(proc_ctx);\n\n",
      map_name, fn_name);
    free(fn_name);

    return map_name;
  }
  else
  {
    APPEND_ARGS(instructions, "  %s(proc_ctx);\n\n", fn_name);
    free(fn_name);

    return NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

static char * function(char * args, char * aisle, char * fn_name)
{
  char * map_name = next_name();

  if(fn_name == NULL)
  {
    fn_name = next_name();

    APPEND_ARGS(functions,
      "static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * map, CMAP_LIST * args)\n{\n%s",
      fn_name, def_n_global_env_decl_());
    char is_return = cmap_parser_part_public.is_return(),
      * instructions = cmap_parser_part_public.pop_instructions();
    APPEND(functions, instructions);
    free(instructions);
    APPEND_ARGS(functions, "%s}\n\n", is_return ? "" : "  return NULL;\n");
  }

  APPEND_ARGS(instructions,
    "  CMAP_MAP * %s = (CMAP_MAP *)cmap_fn(%s, proc_ctx, ",
    map_name, fn_name);
  add_aisle(aisle);
  APPEND(instructions, ");\n");

  free(fn_name);

  APPEND_ARGS(instructions,
    "  cmap_copy_map(cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx),\n"
    "    cmap_definitions(proc_ctx));\n", map_name);

  if(args != NULL)
  {
    APPEND_ARGS(instructions,
      "  cmap_add_arg_names((CMAP_FN *)%s, proc_ctx", map_name);
    add_args(args);
  }
  else APPEND(instructions, "\n");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void c_impl(char * impl)
{
  APPEND_ARGS(instructions, "  %s\n\n", impl);
  free(impl);
}

/*******************************************************************************
*******************************************************************************/

static void if_(char * bool_name)
{
  char is_return = cmap_parser_part_public.is_return(),
    * instructions = cmap_parser_part_public.pop_instructions();

  if(is_return) cmap_parser_part_public.return_();

  APPEND_ARGS(instructions, "  if(%s)\n  {\n", bool_name);
  free(bool_name);
  APPEND(instructions, instructions);
  free(instructions);
  APPEND(instructions, "  }\n");
}

static void else_empty()
{
  APPEND(instructions, "\n");
}

static void else_if()
{
  APPEND(instructions, "  else ");
}

static void else_()
{
  char is_return = cmap_parser_part_public.is_return(),
    * instructions = cmap_parser_part_public.pop_instructions();

  if(is_return) cmap_parser_part_public.return_();

  APPEND(instructions, "  else\n  {\n");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND(instructions, "  }\n\n");
}

/*******************************************************************************
*******************************************************************************/

static char * cmp_to_bool(char * cmp_name, char * op)
{
  char * bool_name = next_name();
  APPEND_ARGS(instructions, "  char %s = (%s %s 0);\n",
    bool_name, cmp_name, op);
  free(cmp_name);
  return bool_name;
}

#define CMP_IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  char * cmp_name = next_name(); \
 \
  APPEND_ARGS(instructions, "  int %s = cmap_cmp(%s, %s);\n", \
    cmp_name, map_l, map_r); \
  free(map_l); \
  free(map_r); \
 \
  return cmp_to_bool(cmp_name, #op); \
}

CMAP_PARSER_UTIL_CMP_LOOP(CMP_IMPL)

static char * cmp_unique(char * map)
{
  char * cmp_name = next_name();

  APPEND_ARGS(instructions,
    "  int %s;\n"
    "  if(cmap_nature(%s) == CMAP_INT_NATURE)\n"
    "    %s = (cmap_int_get((CMAP_INT *)%s) == 0) ? 0 : 1;\n"
    "  else %s = (%s == NULL) ? 0 : 1;\n",
    cmp_name, map, cmp_name, map, cmp_name, map);
  free(map);

  return cmp_to_bool(cmp_name, "!=");
}

/*******************************************************************************
*******************************************************************************/

#define CMP_SET(name, op) name,

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
  arg_names,
  arg_names_push,
  args_map,
  args_map_push,
  map_args,
  list_args,
  string,
  int_,
  return_,
  process,
  process_fn,
  process_c,
  function,
  c_impl,
  if_,
  else_empty,
  else_if,
  else_,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET)
  cmp_unique
};
