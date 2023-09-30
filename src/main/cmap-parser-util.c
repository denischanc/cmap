
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-parser-part.h"
#include "cmap-kv.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS(part, txt, args...) \
  cmap_string_public.append_args(cmap_parser_part_public.part(), txt, args)

#define APPEND(part, txt) \
  cmap_string_public.append(cmap_parser_part_public.part(), txt)

#define APPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION(txt) cmap_parser_part_public.add_instruction(txt)

#define APPEND_LF() cmap_parser_part_public.add_lf()

#define APPEND_INSTRUCTION_AA_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_aisle(&instruction, aisle); \
  add_args(&instruction, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION_AISLE_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  add_aisle(&instruction, aisle); \
  cmap_string_public.append(&instruction, ");"); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION_ARGS_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_args(&instruction, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define PREPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_parser_part_public.prepend_instruction(instruction); \
  free(instruction); \
  instruction = NULL

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
  snprintf(buffer, sizeof(buffer), "cmap_gen_name__%d", id++);
  return strdup(buffer);
}

/*******************************************************************************
*******************************************************************************/

static void add_definitions()
{
  char * instruction = NULL;
  cmap_string_public.append_args(&instruction,
    "CMAP_MAP * %s = cmap_definitions(proc_ctx);", DEFINITIONS_VAR_NAME);
  cmap_parser_part_public.add_definitions(instruction);
  free(instruction);
}

/*******************************************************************************
*******************************************************************************/

static void add_global_env()
{
  char * instruction = NULL;
  cmap_string_public.append_args(&instruction,
    "CMAP_MAP * %s = cmap_global_env(proc_ctx);", GLOBAL_ENV_VAR_NAME);
  cmap_parser_part_public.add_global_env(instruction);
  free(instruction);
}

/*******************************************************************************
*******************************************************************************/

static void add_args(char ** instruction, char * args)
{
  if(args != NULL)
  {
    cmap_string_public.append_args(instruction, ", %s", args);
    free(args);
  }

  cmap_string_public.append(instruction, ", NULL);");
}

static void add_aisle(char ** instruction, char * aisle)
{
  if(aisle != NULL)
  {
    if(!strcmp(aisle, "local"))
      cmap_string_public.append(instruction, "CMAP_AISLE_LOCAL");
    else if(!strcmp(aisle, "global"))
      cmap_string_public.append(instruction, "CMAP_AISLE_GLOBAL");
    else cmap_string_public.append_args(instruction, "\"%s\"", aisle);
    free(aisle);
  }
  else cmap_string_public.append(instruction, "NULL");
}

/*******************************************************************************
*******************************************************************************/

static void function_c(char * name, char is_static)
{
  APPEND_ARGS(main, "%s%s %s(CMAP_PROC_CTX * proc_ctx)\n{\n",
    is_static ? "static " : "",
    cmap_parser_part_public.is_return() ? "CMAP_MAP *" : "void", name);

  if(!is_static)
  {
    APPEND_ARGS(headers,
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
  const char * tmp = cmap_kv_get(cmap_parser_part_public.name2map(), name);
  if(tmp != NULL) return strdup(tmp);
  else
  {
    char * map_name = next_name(), * instruction = NULL;

    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
    add_definitions();
    APPEND_INSTRUCTION_ARGS("if((%s != NULL) && cmap_is_key(%s, \"%s\"))",
      DEFINITIONS_VAR_NAME, DEFINITIONS_VAR_NAME, name);
    APPEND_INSTRUCTION_ARGS(SPACE "%s = cmap_get(%s, \"%s\");",
      map_name, DEFINITIONS_VAR_NAME, name);
    add_global_env();
    APPEND_INSTRUCTION_ARGS("else %s = cmap_get(%s, \"%s\");",
      map_name, GLOBAL_ENV_VAR_NAME, name);
    APPEND_LF();

    free(name);

    return map_name;
  }
}

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, \"%s\");", map_name, map, name);
  APPEND_LF();

  free(map);
  free(name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void set_local(char * name, char * map)
{
  cmap_kv_put(cmap_parser_part_public.name2map(), name, map);

  char * instruction = NULL;
  add_definitions();
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);",
    DEFINITIONS_VAR_NAME, name, map);
  APPEND_LF();

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(char * name, char * map)
{
  const char * tmp = cmap_kv_get(cmap_parser_part_public.name2map(), name);
  char * instruction = NULL;

  if(tmp != NULL)
  {
    cmap_kv_put(cmap_parser_part_public.name2map(), name, map);

    APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);",
      DEFINITIONS_VAR_NAME, name, map);
  }
  else
  {
    add_definitions();
    APPEND_INSTRUCTION_ARGS("if((%s != NULL) && cmap_is_key(%s, \"%s\"))",
      DEFINITIONS_VAR_NAME, DEFINITIONS_VAR_NAME, name);
    APPEND_INSTRUCTION_ARGS(SPACE "cmap_set(%s, \"%s\", %s);",
      DEFINITIONS_VAR_NAME, name, map);
    add_global_env();
    APPEND_INSTRUCTION_ARGS("else cmap_set(%s, \"%s\", %s);",
      GLOBAL_ENV_VAR_NAME, name, map);
  }
  APPEND_LF();

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", src, name, map);
  APPEND_LF();

  free(src);
  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static char * args_push(char * list, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * arg_names(char * name)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "\"%s\"", name);

  free(name);

  return ret;
}

static char * arg_names_push(char * list, char * name)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, \"%s\"", list, name);

  free(list);
  free(name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "\"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

static char * args_map_push(char * list, char * name, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, \"%s\", %s", list, name, map);

  free(list);
  free(name);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * map_args(char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS("%s = cmap_to_map(proc_ctx, ", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS(
    "%s = (CMAP_MAP *)cmap_to_list(proc_ctx, ", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx, ", map_name, string);
  APPEND_LF();

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_int(%s, proc_ctx, ", map_name, i);
  APPEND_LF();

  free(i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("return %s;", map);
  free(map);

  cmap_parser_part_public.return_();
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? name(fn_name) : path(map, fn_name),
    * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s",
    map_name, map_fn, map_keep);
  APPEND_LF();

  free(map_fn);
  free(map_keep);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", map_name, fn);
  APPEND_LF();

  free(fn);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  char * instruction = NULL;

  if(need_ret)
  {
    char * map_name = next_name();

    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
    APPEND_INSTRUCTION_ARGS("%s = %s(proc_ctx);", map_name, fn_name);
    APPEND_LF();

    free(fn_name);

    return map_name;
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("%s(proc_ctx);", fn_name);
    APPEND_LF();

    free(fn_name);

    return NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

static char * function(char * args, char * aisle, char * fn_name)
{
  char * map_name = next_name(), * instruction = NULL;

  if(fn_name == NULL)
  {
    fn_name = next_name();

    APPEND_ARGS(functions,
      "static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * map, CMAP_LIST * args)\n{\n", fn_name);
    char is_return = cmap_parser_part_public.is_return(),
      * instructions = cmap_parser_part_public.pop_instructions();
    APPEND(functions, instructions);
    free(instructions);
    APPEND_ARGS(functions, "%s}\n\n", is_return ? "" : "  return NULL;\n");
  }

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx, ", map_name, fn_name);

  free(fn_name);

  add_definitions();
  APPEND_INSTRUCTION_ARGS(
    "cmap_copy_map(cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx), %s);",
    map_name, DEFINITIONS_VAR_NAME);

  if(args != NULL)
  {
    APPEND_INSTRUCTION_ARGS_ARGS(
      "cmap_add_arg_names((CMAP_FN *)%s, proc_ctx", map_name);
  }
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void c_impl(char * impl)
{
  APPEND_ARGS(instructions, "  %s\n\n", impl);
  free(impl);
}

static void c_impl_root(char * impl)
{
  APPEND(c_impl, impl);
  APPEND(c_impl, "\n\n");
  free(impl);
}

/*******************************************************************************
*******************************************************************************/

static char prev_else = (1 == 0);

static void if_(char * bool_name)
{
  char is_return = cmap_parser_part_public.is_return(),
    * instructions = cmap_parser_part_public.pop_instructions();

  if(is_return) cmap_parser_part_public.return_();

  if(prev_else) prev_else = (1 == 0);
  else cmap_parser_part_public.add_prefix();

  cmap_string_public.append_args(cmap_parser_part_public.instructions(),
    "if(%s)\n", bool_name);
  free(bool_name);
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
}

static void else_empty()
{
  APPEND_LF();
}

static void else_if()
{
  cmap_parser_part_public.add_prefix();
  cmap_string_public.append(cmap_parser_part_public.instructions(), "else ");
  prev_else = (1 == 1);
}

static void else_()
{
  char is_return = cmap_parser_part_public.is_return(),
    * instructions = cmap_parser_part_public.pop_instructions();

  if(is_return) cmap_parser_part_public.return_();

  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

static char * cmp_to_bool(char * cmp_name, char * op)
{
  char * bool_name = next_name(), * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("char %s = (%s %s 0);", bool_name, cmp_name, op);
  free(cmp_name);
  return bool_name;
}

#define CMP_IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  char * cmp_name = next_name(), * instruction = NULL; \
 \
  APPEND_INSTRUCTION_ARGS("int %s = cmap_cmp(%s, %s);", \
    cmp_name, map_l, map_r); \
  free(map_l); \
  free(map_r); \
 \
  return cmp_to_bool(cmp_name, #op); \
}

CMAP_PARSER_UTIL_CMP_LOOP(CMP_IMPL)

static char * cmp_unique(char * map)
{
  char * cmp_name = next_name(), * instruction = NULL;

  APPEND_INSTRUCTION_ARGS("int %s;", cmp_name);
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = (cmap_int_get((CMAP_INT *)%s) == 0) ? 0 : 1;", cmp_name, map);
  APPEND_INSTRUCTION_ARGS("else %s = (%s == NULL) ? 0 : 1;", cmp_name, map);
  free(map);

  return cmp_to_bool(cmp_name, "!=");
}

/*******************************************************************************
*******************************************************************************/

static char * new(char * map, char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS(
    "%s = cmap_new((CMAP_FN *)%s, proc_ctx, ", map_name, map);
  APPEND_LF();

  free(map);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

#define CMP_SET(name, op) name,

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
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
  c_impl_root,
  if_,
  else_empty,
  else_if,
  else_,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET)
  cmp_unique,
  new
};
